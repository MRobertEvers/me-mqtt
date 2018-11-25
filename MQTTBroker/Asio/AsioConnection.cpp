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
AsioConnection::WriteAsync( std::shared_ptr<std::string const> apszMsg )
{
   // Since this function is available from outside the callbacks
   // of the strand, we use the stand to protect the queue.
   auto pWriteCB = [this, self = shared_from_this(), msg = apszMsg]()->void
   {
      bool bInflight = !m_qOutQueue.empty();
      m_qOutQueue.push( msg );
      if( !bInflight )
      {
         queueWrite();
      }
   };

   asio::post(
      m_pSock->get_io_context().get_executor(),
      m_pStrand->wrap(pWriteCB)
   );
}

void
AsioConnection::WriteAsync( std::string const& aszMsg )
{
   WriteAsync( aszMsg.data(), aszMsg.size() );
}

void
AsioConnection::WriteAsync( char const* apBuf, size_t aNumBytes )
{
   // Copy the data.
   auto pByteBuf = std::make_shared<std::string>( apBuf, apBuf+aNumBytes );
   WriteAsync( pByteBuf );
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
   auto recvCB = 
      [this, self = shared_from_this()]
   ( std::error_code ec, std::size_t bytes_transferred )
   {
      self->onReceiveBytes( ec, bytes_transferred );
   };

   // Normally, the Receiver doesn't need to use a strand because we handle the 
   // incoming bytes before we start listening again.
   // But! Other objects use the strand to interact with this connection.
   m_pSock->async_receive(
      *m_pMutableBuffer,
      recvCB
   );
}

void
AsioConnection::awaitReceive()
{
   resetBuffer();
   resetReceive();
}

void
AsioConnection::queueWrite()
{
   // Resets the async_writer.
   auto writeCB = 
      [this, self = shared_from_this()]
   ( std::error_code ec, std::size_t aBytesWritten )
   {
      if( !ec )
      {
         m_qOutQueue.pop();
         if( !m_qOutQueue.empty() )
         {
            queueWrite();
         }
      }
      else
      {
         // TODO: Check if the error indeicates that the socket is closed.
         // This will stop the write cycle...
      }
   };

   asio::async_write( 
      *m_pSock,
      asio::buffer( m_qOutQueue.front()->data(), m_qOutQueue.front()->length() ),
      m_pStrand->wrap(writeCB)
   );
}
