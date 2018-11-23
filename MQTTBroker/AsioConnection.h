#pragma once
#include "asio.hpp"
#include "ServerIOStream.h"
#include "FixedHeader.h"
#include "RingBuffer.h"

#define MAX_BUFFER 512

class AsioConnection
{
public:
   AsioConnection( std::shared_ptr<asio::ip::tcp::socket> apSock );
   ~AsioConnection();

   virtual void OnReceiveBytes( char const* apBytes, size_t aNumBytes ) = 0;

protected:
   virtual void onReceiveBytes( const asio::error_code& aec, size_t aNumBytes );
   virtual void resetBuffer();
   virtual void resetReceive();
   virtual void awaitReceive();

private:
   char* m_pByteBuf;
   std::shared_ptr<asio::mutable_buffer> m_pMutableBuffer;

   std::shared_ptr<asio::ip::tcp::socket> m_pSock;
};

