#include "stdafx.h"
#include "MQTTConnection.h"


MQTTConnection::MQTTConnection( std::shared_ptr<asio::ip::tcp::socket> apSock, AsioConnectionManager& aManager, std::shared_ptr<ServerIOStream> apOStream )
   : m_iHaveBytes(0), m_iNeedBytes(0), m_eState(FIXED_HEADER_FLAGS),
   m_pIOStream(apOStream),
   AsioConnection( apSock, aManager )
{
   m_pConnectTimer = std::shared_ptr<asio::steady_timer>( new asio::steady_timer( 
      apSock->get_io_context()
   ) );


   m_iNeedBytes = 1;
}


MQTTConnection::~MQTTConnection()
{
   m_pConnectTimer->cancel();
   *m_pIOStream << "Deleted MQTTConnection" << std::endl;
}

void 
MQTTConnection::OnReceiveBytes( char const* apBytes, size_t aNumBytes )
{
   static size_t iG = 0;
   m_szBuf.append( apBytes, aNumBytes );

   size_t bufSize = m_szBuf.size();
   while( iG + m_iNeedBytes <= bufSize )
   {
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
               throw;
            }
            m_szCurrentMessage += iLenByte;
            if( (iLenByte & 0x80) == 0 )
            {
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
            memcpy_s( &m_szCurrentMessage[len], m_iNeedBytes, &m_szBuf[iG], m_iNeedBytes );
            m_eState = FIXED_HEADER_MESSAGE_SIZE;
            iG += m_iNeedBytes;

            // *m_pIOStream << m_szCurrentMessage << std::endl;

            // Reset message reader state.
            m_pConnectTimer->cancel(); // We have received a message.
            m_szBuf = m_szBuf.substr( iG );
            m_iNeedBytes = 1;
            iG = 0;
            bufSize = m_szBuf.size();
         }
         break;
      }
   }
}

void 
MQTTConnection::Start()
{
   AsioConnection::Start();
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

void MQTTConnection::Stop()
{
   AsioConnection::Stop();
   m_pConnectTimer->cancel();
}

void
MQTTConnection::onConnectTimer( const asio::error_code& ec )
{
   if( ec )
   {
      *m_pIOStream << "Message received within time limit." << std::endl;
      //*m_pIOStream << "[" << std::this_thread::get_id()
      //   << "] Error: " << ec << ", " << ec.message() << std::endl;
   }
   else
   {
      *m_pIOStream << "Closing" << std::endl;
   }
}
