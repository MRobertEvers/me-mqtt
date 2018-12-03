#pragma once
#include <memory>

class AsioConnection;

class IAsioConnectionFactory
{
public:
   virtual std::shared_ptr<AsioConnection>
      NewConnection( std::shared_ptr<asio::ip::tcp::socket> sock ) = 0;
};
