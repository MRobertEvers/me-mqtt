#pragma once
#include "asio.hpp"
#include "ServerIOStream.h"
#include "RingBuffer.h"
#include <queue>
#include <string>

#define MAX_BUFFER 512

class AsioConnectionManager;
class AsioConnection : public std::enable_shared_from_this<AsioConnection>
{
public:
   AsioConnection( std::shared_ptr<asio::ip::tcp::socket> apSock );
   virtual ~AsioConnection();

   virtual void OnReceiveBytes( char const* apBytes, size_t aNumBytes ) = 0;
   std::shared_ptr<asio::io_context::strand> GetStrand();

   virtual void Start( AsioConnectionManager* aManager );
   virtual void Stop();

   virtual void SetError( const asio::error_code& aec );
   virtual asio::error_code GetLastError() const;

   void WriteAsync( std::shared_ptr<std::string const> apszMsg );
   void WriteAsync( std::string const& aszMsg );
   void WriteAsync( char const* apBuf, size_t aNumBytes );

   // 
   void ManagerClose();

protected:
   virtual void onReceiveBytes( const asio::error_code& aec, size_t aNumBytes );
   virtual void onSentBytes( const asio::error_code& aec, size_t aNumBytes );
   virtual void resetBuffer();
   virtual void resetReceive();
   virtual void awaitReceive();
   virtual void queueWrite();

private:
   char* m_pByteBuf;
   std::shared_ptr<asio::mutable_buffer> m_pMutableBuffer;

   std::shared_ptr<asio::ip::tcp::socket> m_pSock;
   std::shared_ptr<asio::io_context::strand> m_pStrand;
   AsioConnectionManager* m_Manager;
   asio::error_code m_LastError;

   std::queue<std::shared_ptr<std::string const>> m_qOutQueue;
};

