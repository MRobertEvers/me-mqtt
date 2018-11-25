#pragma once
#include "AsioConnectionFactory.h"
#include <set>
#include <memory>

class ServerIOStream;
class AsioService;

namespace me
{
class Broadcaster;

class MQTTConnectionFactory : public AsioConnectionFactory
{
public:
   MQTTConnectionFactory( 
      std::shared_ptr<ServerIOStream> aIOStream,
      std::shared_ptr<AsioService> apIOService );
   ~MQTTConnectionFactory();

   virtual std::shared_ptr<AsioConnection>
      NewConnection( std::shared_ptr<asio::ip::tcp::socket> sock ) override;

private:
   std::shared_ptr<ServerIOStream> m_pIOStream;
   std::shared_ptr<Broadcaster> m_pBroadcaster;
};
}