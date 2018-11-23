#include "stdafx.h"
#include "AsioConnection.h"
#include "AsioConnectionManager.h"

AsioConnection::AsioConnection( std::shared_ptr<asio::ip::tcp::socket> apSock )
   : m_pByteBuf(new char[MAX_BUFFER]), m_pSock(apSock),
   m_pMutableBuffer(new asio::mutable_buffer(m_pByteBuf, MAX_BUFFER)),
   m_pStrand( new asio::io_context::strand( apSock->get_io_context() ) )
{

}


AsioConnection::~AsioConnection()
{
   delete[] m_pByteBuf;
}

std::shared_ptr<asio::io_context::strand>
AsioConnection::GetStrand()
{
   return m_pStrand;
}

void
AsioConnection::Start( AsioConnectionManager* aManager )
{
   m_Manager = aManager;
   awaitReceive();
}

void
AsioConnection::Stop()
{
   m_Manager->CloseConnection( shared_from_this() );
}

void
AsioConnection::ManagerClose()
{
   m_pSock->close();
}

void 
AsioConnection::onReceiveBytes( 
   const asio::error_code & aec, 
   size_t aNumBytes )
{
   if( aec )
   {
      
   }
   else
   {
      awaitReceive();
      OnReceiveBytes( m_pByteBuf, aNumBytes );
   }
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
      m_pStrand->wrap( [this, self=shared_from_this()]( std::error_code ec, std::size_t bytes_transferred )
         {
            this->onReceiveBytes( ec, bytes_transferred );
         })
   );
}

void
AsioConnection::awaitReceive()
{
   resetBuffer();
   resetReceive();
}