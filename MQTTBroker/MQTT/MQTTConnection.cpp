#include "stdafx.h"
#include "MQTTConnection.h"
#include "MalformedFixedHeader.h"
#include "MalformedPacket.h"
#include "Broker\BrokerSession.h"
#include "Broker\Broadcaster.h"
#include "Broker\BroadcasterClient.h"
#include "Broker\BrokerClient.h"

namespace me
{
const MQTTConnection::StateVars MQTTConnection::StateVars::resetter;

MQTTConnection::MQTTConnection(
   std::shared_ptr<asio::ip::tcp::socket> apSock,
   std::shared_ptr<Broadcaster> apBroadcaster,
   std::shared_ptr<ServerIOStream> apOStream )
   : m_pIOStream( apOStream ), m_pBroadcaster(apBroadcaster), AsioConnection( apSock )
{
   m_pszCurrentMessage = new std::string;
   m_pClient = std::make_shared<BrokerSession>( apBroadcaster, this );
   m_pConnectTimer = std::shared_ptr<asio::steady_timer>( new asio::steady_timer(
      apSock->get_io_context()
   ) );
}


MQTTConnection::~MQTTConnection()
{
   *m_pIOStream << "Deleted MQTTConnection" << std::endl;
}

void
MQTTConnection::OnReceiveBytes( char const* apBytes, size_t aNumBytes )
{
   m_szBuf.append( apBytes, aNumBytes );

   while( m_State.iReadIndex + m_State.iNeedBytes <= m_szBuf.size() )
   {
      try
      {
         handleBytes();
      }
      catch( MalformedFixedHeader )
      {
         *m_pIOStream << "Bad Fixed Header";
         Stop();
      }
      catch( MalformedPacket )
      {
         *m_pIOStream << "Bad Packet";
         Stop();
      }
   }
}

void
MQTTConnection::Start( AsioConnectionManager* aManager )
{
   AsioConnection::Start( aManager );
   m_pConnectTimer->expires_after(
      std::chrono::seconds( 5 )
   );

   m_pConnectTimer->async_wait( GetStrand()->wrap(
      [this, self = shared_from_this()]( std::error_code ec )
   {
      this->onConnectTimer( ec );
   } )
   );
}

void
MQTTConnection::Stop()
{
   auto ec = GetLastError();
   *m_pIOStream << "[" << std::this_thread::get_id()
      << "] Error: " << ec << ", " << ec.message() << std::endl;
   AsioConnection::Stop();
   m_pConnectTimer->cancel();
}


void
MQTTConnection::dispatchMessage(
   me::pcstring aszData,
   unsigned char aiFixedHeaderSize )
{
   // Validate as much of the input that we can.
   char const* data = aszData->data();
   size_t size = aszData->size();
   if( size < 2 )
   {
      throw MalformedFixedHeader();
   }

   unsigned char iTypeAndFlags = data[0];
   unsigned char iType = (iTypeAndFlags & 0xF0) >> 4;
   if( iType == 0 || iType >= 0xF )
   {
      throw MalformedFixedHeader();
   }

   // Dispatch.
   ControlPacket::PacketTypes type = (ControlPacket::PacketTypes)iType;
   switch( type )
   {
   case ControlPacket::PacketTypes::CONNECT:
      m_pClient->HandleConnect(
         std::make_shared<ConnectPacket>( aszData, aiFixedHeaderSize )
      );
      break;
   case ControlPacket::PacketTypes::PUBLISH:
      m_pClient->HandlePublish(
         std::make_shared<PublishPacket>( aszData, aiFixedHeaderSize )
      );
      break;
   case ControlPacket::PacketTypes::PUBACK:
      m_pClient->HandlePuback(
         std::make_shared<PubackPacket>( aszData, aiFixedHeaderSize )
      );
      break;
   case ControlPacket::PacketTypes::PUBREC:
      m_pClient->HandlePubrec(
         std::make_shared<PubrecPacket>( aszData, aiFixedHeaderSize )
      );
      break;
   case ControlPacket::PacketTypes::PUBREL:
      m_pClient->HandlePubrel(
         std::make_shared<PubrelPacket>( aszData, aiFixedHeaderSize )
      );
      break;
   case ControlPacket::PacketTypes::PUBCOMP:
      m_pClient->HandlePubcomp(
         std::make_shared<PubcompPacket>( aszData, aiFixedHeaderSize )
      );
      break;
   case ControlPacket::PacketTypes::SUBSCRIBE:
      m_pClient->HandleSubscribe(
         std::make_shared<SubscribePacket>( aszData, aiFixedHeaderSize )
      );
      break;
   case ControlPacket::PacketTypes::UNSUBSCRIBE:
      m_pClient->HandleUnsubscribe(
         std::make_shared<UnsubscribePacket>( aszData, aiFixedHeaderSize )
      );
      break;
   case ControlPacket::PacketTypes::PINGREQ:
      m_pClient->HandlePingReq(
         std::make_shared<PingReqPacket>()
      );
      break;
   case ControlPacket::PacketTypes::DISCONNECT:
      m_pClient->HandleDisconnect(
         std::make_shared<DisconnectPacket>()
      );
      break;
   default:
      Stop();
      break;
   }
}

void
MQTTConnection::handleBytes()
{
   switch( m_State.iState )
   {
   case FIXED_HEADER_FLAGS:
      {
         char iTypeAndFlags;
         memcpy_s( &iTypeAndFlags, 1, &m_szBuf[m_State.iReadIndex++], 1 );
         m_State.iState = FIXED_HEADER_MESSAGE_SIZE;
         m_State.iNeedBytes = 1;
         *m_pszCurrentMessage += iTypeAndFlags;
      }
      break;
   case FIXED_HEADER_MESSAGE_SIZE:
      {
         memcpy_s(
            &m_State.iMessageLenByte, 1,
            &m_szBuf[m_State.iReadIndex++], 1
         );
         m_State.iMessageLenValue +=
            (m_State.iMessageLenByte & 0x7F) * m_State.iMessageLenMultiplier;
         m_State.iMessageLenMultiplier = m_State.iMessageLenMultiplier << 7;
         if( m_State.iMessageLenMultiplier > 128 * 128 * 128 )
         {
            throw MalformedFixedHeader();
         }
         m_pszCurrentMessage->append( 1, m_State.iMessageLenByte );
         if( (m_State.iMessageLenByte & 0x80) == 0 )
         {
            m_State.iFixedHeaderSize = m_pszCurrentMessage->size();
            m_State.iState = MESSAGE_PAYLOAD;
            m_State.iNeedBytes = m_State.iMessageLenValue;
         }
      }
      if( m_State.iNeedBytes != 0 )
      {
         break;
      }
   case MESSAGE_PAYLOAD:
      {
         size_t len = m_pszCurrentMessage->size();
         m_pszCurrentMessage->append( m_State.iNeedBytes, ' ' );
         memcpy_s(
            &(*m_pszCurrentMessage)[len], m_State.iNeedBytes,
            &m_szBuf[m_State.iReadIndex], m_State.iNeedBytes
         );
         m_State.iState = FIXED_HEADER_MESSAGE_SIZE;
         m_State.iReadIndex += m_State.iNeedBytes;
         m_pConnectTimer->cancel(); // We have received a message.

         *m_pIOStream << *m_pszCurrentMessage << std::endl;
         dispatchMessage( 
            std::shared_ptr<const std::string>(m_pszCurrentMessage),
            m_State.iFixedHeaderSize 
         );

         // Reset message reader state.
         m_szBuf = m_szBuf.substr( m_State.iReadIndex );
         m_State.Reset();
         m_pszCurrentMessage = new std::string();
      }
      break;
   }
}


void
MQTTConnection::onConnectTimer( const asio::error_code& ec )
{
   if( ec )
   {
      SetError( ec );
      *m_pIOStream << "Message received within time limit." << std::endl;
      //*m_pIOStream << "[" << std::this_thread::get_id()
      //   << "] Error: " << ec << ", " << ec.message() << std::endl;
   }
   else
   {
      *m_pIOStream << "Closing" << std::endl;
   }
}
}