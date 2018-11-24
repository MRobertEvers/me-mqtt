#pragma once
#include <memory>

class AsioConnection;

class AsioConnectionFactory
{
public:
   AsioConnectionFactory();
   virtual ~AsioConnectionFactory();

   virtual std::shared_ptr<AsioConnection>
      NewConnection( std::shared_ptr<asio::ip::tcp::socket> sock ) = 0;
};
