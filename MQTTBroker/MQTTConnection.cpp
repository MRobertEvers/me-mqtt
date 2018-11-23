#include "stdafx.h"
#include "MQTTConnection.h"


MQTTConnection::MQTTConnection( std::shared_ptr<asio::ip::tcp::socket> apSock, std::shared_ptr<ServerIOStream> apOStream )
   : m_iHaveBytes(0), m_iNeedBytes(0), m_eState(FIXED_HEADER_FLAGS), m_pIOStream(apOStream), AsioConnection( apSock )
{
   m_iNeedBytes = 1;
}


MQTTConnection::~MQTTConnection()
{
}

void 
MQTTConnection::OnReceiveBytes( char const* apBytes, size_t aNumBytes )
{
   static size_t iG = 0;
   m_szBuf.append( apBytes, aNumBytes );

   char iTypeAndFlags;
   while( iG + m_iNeedBytes <= m_szBuf.size() )
   {
      switch( m_eState )
      {
      case FIXED_HEADER_FLAGS:
         memcpy_s( &iTypeAndFlags, 1, &m_szBuf[iG++], 1 );
         m_eState = FIXED_HEADER_MESSAGE_SIZE;
         m_iNeedBytes = 1;
         m_szCurrentMessage += iTypeAndFlags;
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
         size_t len = m_szCurrentMessage.size();
         m_szCurrentMessage.append( m_iNeedBytes, ' ' );
         memcpy_s( &m_szCurrentMessage[len], m_iNeedBytes, &m_szBuf[iG], m_iNeedBytes );
         m_eState = FIXED_HEADER_MESSAGE_SIZE;
         iG += m_iNeedBytes;

         *m_pIOStream << m_szCurrentMessage << std::endl;

         // Reset message reader state.
         m_szBuf = m_szBuf.substr( iG );
         m_iNeedBytes = 1;
         iG = 0;
         break;
      }
   }
}
