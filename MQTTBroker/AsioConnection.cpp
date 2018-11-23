#include "stdafx.h"
#include "AsioConnection.h"


AsioConnection::AsioConnection( std::shared_ptr<asio::ip::tcp::socket> apSock )
   : m_pByteBuf(new char[MAX_BUFFER]), m_pSock(apSock), m_pMutableBuffer(new asio::mutable_buffer(m_pByteBuf, MAX_BUFFER))
{
   awaitReceive();
}


AsioConnection::~AsioConnection()
{
   delete[] m_pByteBuf;
}

void 
AsioConnection::onReceiveBytes( 
   const asio::error_code & aec, 
   size_t aNumBytes )
{
   OnReceiveBytes(m_pByteBuf, aNumBytes);

   awaitReceive();
}

void
AsioConnection::resetBuffer()
{
   memset( m_pByteBuf, 0, MAX_BUFFER );
}

void 
AsioConnection::resetReceive()
{
   m_pSock->async_receive( 
      *m_pMutableBuffer, 
      std::bind( 
         &AsioConnection::onReceiveBytes,
         this, 
         std::placeholders::_1, 
         std::placeholders::_2 
      ) 
   );
}

void
AsioConnection::awaitReceive()
{
   resetBuffer();
   resetReceive();
}
