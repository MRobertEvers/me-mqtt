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
AsioConnection::SetError( const asio::error_code & aec )
{
   m_LastError = aec;
}

asio::error_code
AsioConnection::GetLastError() const
{
   return m_LastError;
}

void
AsioConnection::WriteAsync( std::string&& aszMsg )
{
   WriteAsync( aszMsg.data(), aszMsg.size() );
}

void
AsioConnection::WriteAsync( std::string const & aszMsg )
{
   WriteAsync( aszMsg.data(), aszMsg.size() );
}

void
AsioConnection::WriteAsync( char const* apBuf, size_t aNumBytes )
{
   // Copy the data.
   auto pByteBuf = std::make_shared<std::vector<char>>( apBuf, apBuf+aNumBytes );
   auto pBuf = std::shared_ptr<asio::const_buffer>( new asio::const_buffer(pByteBuf->data(), pByteBuf->size()) );
   m_pSock->async_send(
      *pBuf,
      [this, self = shared_from_this(), pBuf, pByteBuf]( const asio::error_code & aec, size_t aNumBytes )
      {
         onSentBytes( aec, aNumBytes );
      } 
   );
}

void 
AsioConnection::Write( char const * apBuf, size_t aNumBytes )
{
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
      SetError( aec );
      Stop();
   }
   else
   {
      OnReceiveBytes( m_pByteBuf, aNumBytes );

      awaitReceive();
   }
}

void 
AsioConnection::onSentBytes( const asio::error_code & aec, size_t aNumBytes )
{
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