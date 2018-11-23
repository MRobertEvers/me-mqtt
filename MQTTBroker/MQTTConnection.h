#pragma once
#include "asio.hpp"
#include "asio/basic_waitable_timer.hpp"
#include "AsioConnection.h"
#include "ServerIOStream.h"
#include <string>
#include <sstream>

class MQTTConnection: public AsioConnection
{
public:
   MQTTConnection( 
      std::shared_ptr<asio::ip::tcp::socket> apSock,
      std::shared_ptr<ServerIOStream> apOStream );
   virtual ~MQTTConnection();

   // Inherited via AsioConnection
   virtual void OnReceiveBytes( char const* apBytes, size_t aNumBytes ) override;
   virtual void Start( AsioConnectionManager* aManager ) override;
   virtual void Stop() override;

protected:
   enum State
   {
      FIXED_HEADER_FLAGS = 0x00,
      FIXED_HEADER_MESSAGE_SIZE,
      MESSAGE_PAYLOAD
   };

   void onConnectTimer( const asio::error_code& ec );

private:
   size_t m_iNeedBytes;
   size_t m_iHaveBytes;

   State m_eState;

   std::shared_ptr<ServerIOStream> m_pIOStream;
   std::string m_szCurrentMessage;
   std::string m_szBuf;

   std::shared_ptr<asio::steady_timer> m_pConnectTimer;
};

