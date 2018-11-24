#include "stdafx.h"
#include "MQTTConnection.h"
#include "MalformedFixedHeader.h"
#include "PacketFactory.h"
#include "Connect\ConnectPacket.h"
#include "Connack\ConnackPacket.h"
#include "Broker\BrokerClient.h"


MQTTConnection::MQTTConnection(
   std::shared_ptr<asio::ip::tcp::socket> apSock,
   std::shared_ptr<ServerIOStream> apOStream )
   : m_iHaveBytes(0), m_iNeedBytes(0), m_eState(FIXED_HEADER_FLAGS),
   m_pIOStream(apOStream),
   AsioConnection( apSock )
{
   m_pConnectTimer = std::shared_ptr<asio::steady_timer>( new asio::steady_timer( 
      apSock->get_io_context()
   ) );

   m_iNeedBytes = 1;
}


MQTTConnection::~MQTTConnection()
{
   *m_pIOStream << "Deleted MQTTConnection" << std::endl;
}

void
MQTTConnection::OnMessage( ControlPacket* apPacket )
{
   if( !m_pClient )
   {
      // TODO: 
      throw;
   }
   else
   {
      m_pClient->HandlePacket( apPacket );
   }
}

void
MQTTConnection::OnMessage( ConnectPacket* apPacket )
{
   m_pClient = std::shared_ptr<BrokerClient>( new BrokerClient(this) );
   m_pClient->HandleConnect( apPacket );
}

void 
MQTTConnection::OnReceiveBytes( char const* apBytes, size_t aNumBytes )
{
   static size_t iG = 0;
   m_szBuf.append( apBytes, aNumBytes );

   size_t bufSize = m_szBuf.size();
   while( iG + m_iNeedBytes <= bufSize )
   {
      try
      {
         handleBytes( iG, bufSize );
      }
      catch( MalformedFixedHeader )
      {
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


void MQTTConnection::handleBytes( size_t &iG, size_t &bufSize )
{
   static char iFixedHeaderSize = 0;
   switch( m_eState )
   {
   case FIXED_HEADER_FLAGS:
      {
         char iTypeAndFlags;
         memcpy_s( &iTypeAndFlags, 1, &m_szBuf[iG++], 1 );
         m_eState = FIXED_HEADER_MESSAGE_SIZE;
         m_iNeedBytes = 1;
         m_szCurrentMessage += iTypeAndFlags;
      }
      break;
   case FIXED_HEADER_MESSAGE_SIZE:
      {
         static char iLenByte = 0;
         static size_t multiplier = 1;
         static size_t value = 0;
         memcpy_s( &iLenByte, 1, &m_szBuf[iG++], 1 );
         value += (iLenByte & 0x7F) * multiplier;
         multiplier = multiplier << 7;
         if( multiplier > 128 * 128 * 128 )
         {
            throw MalformedFixedHeader();
         }
         m_szCurrentMessage.append( 1, iLenByte );
         if( (iLenByte & 0x80) == 0 )
         {
            iFixedHeaderSize = m_szCurrentMessage.size();
            m_eState = MESSAGE_PAYLOAD;
            m_iNeedBytes = value;
            iLenByte = 0;
            multiplier = 1;
            value = 0;
         }
      }
      break;
   case MESSAGE_PAYLOAD:
      {
         size_t len = m_szCurrentMessage.size();
         m_szCurrentMessage.append( m_iNeedBytes, ' ' );
         memcpy_s( 
            &m_szCurrentMessage[len], m_iNeedBytes, 
            &m_szBuf[iG], m_iNeedBytes 
         );
         m_eState = FIXED_HEADER_MESSAGE_SIZE;
         iG += m_iNeedBytes;

         // *m_pIOStream << m_szCurrentMessage << std::endl;

         // Reset message reader state.
         m_pConnectTimer->cancel(); // We have received a message.
         m_szBuf = m_szBuf.substr( iG );
         m_iNeedBytes = 1;
         iG = 0;
         bufSize = m_szBuf.size();

         // Handle the message.
         OnMessage( PacketFactory::GetPacket( m_szCurrentMessage, iFixedHeaderSize ) );
         iFixedHeaderSize = 0;
         //ConnackPacket pc( 0, 0x00 );
         //WriteAsync( pc.Serialize() );
         //std::string sz;
         //sz += (char)1 << 5;
         //sz += (char)2;
         //sz.append( 2, '\0' );
         //WriteAsync( sz.data(), sz.size() );

         //ConnectPacket pc( m_szCurrentMessage, iFixedHeaderSize );
         //
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
