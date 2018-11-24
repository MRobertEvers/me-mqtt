#pragma once
#include "AsioConnectionFactory.h"
#include <set>
#include <memory>

class ServerIOStream;

class MQTTConnectionFactory : public AsioConnectionFactory
{
public:
   MQTTConnectionFactory( std::shared_ptr<ServerIOStream> aIOStream );
   ~MQTTConnectionFactory();

   virtual std::shared_ptr<AsioConnection>
      NewConnection( std::shared_ptr<asio::ip::tcp::socket> sock ) override;

private:
   std::shared_ptr<ServerIOStream> m_pIOStream;
};
