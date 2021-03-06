#pragma once
#include "asio.hpp"
#include "asio/basic_waitable_timer.hpp"
#include "AsioConnection.h"
#include "Definitions.h"
#include "ServerIOStream.h"
#include <string>
#include <sstream>

namespace me
{
class BrokerClient;
class Broadcaster;
class BrokerSession;

class MQTTConnection : public AsioConnection
{
public:
   MQTTConnection(
      std::shared_ptr<asio::ip::tcp::socket> apSock,
      std::shared_ptr<Broadcaster> apBroadcaster,
      std::shared_ptr<ServerIOStream> apOStream );
   virtual ~MQTTConnection();

   // Inherited via AsioConnection
   virtual void OnReceiveBytes( char const* apBytes, size_t aNumBytes ) override;
   virtual void Start( AsioConnectionManager* aManager ) override;
   virtual void Stop() override;

protected:

   void dispatchMessage( me::pcstring aszData, unsigned char aiFixedHeaderSize );
   void handleBytes();
   void onConnectTimer( const asio::error_code& ec );

private:
   std::shared_ptr<ServerIOStream> m_pIOStream;
   std::string* m_pszCurrentMessage;
   std::string m_szBuf;

   std::shared_ptr<asio::steady_timer> m_pConnectTimer;
   std::shared_ptr<BrokerSession> m_pClient;
   std::shared_ptr<Broadcaster> m_pBroadcaster;

private:
   enum State : unsigned char
   {
      FIXED_HEADER_FLAGS = 0x00,
      FIXED_HEADER_MESSAGE_SIZE,
      MESSAGE_PAYLOAD
   };
   struct StateVars
   {
   private:
      static const StateVars resetter;

   public:
      size_t iNeedBytes = 1;
      size_t iReadIndex = 0;
      size_t iMessageLenMultiplier = 1;
      size_t iMessageLenValue = 0;
      char iMessageLenByte = 0;
      State iState = FIXED_HEADER_FLAGS;
      short iFixedHeaderSize = 0;

      StateVars() {}

      void Reset()
      {
         memcpy_s( this, sizeof( StateVars ), &resetter, sizeof( StateVars ) );
      }
   };

   StateVars m_State;
};
}