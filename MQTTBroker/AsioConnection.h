#pragma once
#include "asio.hpp"
#include "ServerIOStream.h"
#include "FixedHeader.h"
#include "RingBuffer.h"

#define MAX_BUFFER 512
class AsioConnectionManager;
class AsioConnection : public std::enable_shared_from_this<AsioConnection>
{
public:
   AsioConnection( std::shared_ptr<asio::ip::tcp::socket> apSock, AsioConnectionManager& aManager );
   virtual ~AsioConnection();

   virtual void OnReceiveBytes( char const* apBytes, size_t aNumBytes ) = 0;
   std::shared_ptr<asio::io_context::strand> GetStrand();

   virtual void Start();
   virtual void Stop();

   // 
   void ManagerClose();

protected:
   virtual void onReceiveBytes( const asio::error_code& aec, size_t aNumBytes );
   virtual void resetBuffer();
   virtual void resetReceive();
   virtual void awaitReceive();

private:
   char* m_pByteBuf;
   std::shared_ptr<asio::mutable_buffer> m_pMutableBuffer;

   std::shared_ptr<asio::ip::tcp::socket> m_pSock;
   std::shared_ptr<asio::io_context::strand> m_pStrand;

   AsioConnectionManager& m_Manager;
};

