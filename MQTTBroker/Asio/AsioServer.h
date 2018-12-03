#pragma once
#include "asio.hpp"
#include "AsioService.h"
#include "AsioConnection.h"
#include "AsioConnectionManager.h"
#include "IAsioConnectionFactory.h"
#include "ServerIOStream.h"
#include <memory>
#include <vector>

class AsioServer
{
public:
   AsioServer( 
      std::shared_ptr<AsioService> apIOService,
      std::shared_ptr<IAsioConnectionFactory> apFactory,
      std::shared_ptr<ServerIOStream> aIOStream );
   ~AsioServer();

   void StartListener( const std::string& aszHost, const std::string& aszPort );

protected:
   void awaitConnection();

   void handleConnect( 
      const asio::error_code& ec,
      std::shared_ptr<asio::ip::tcp::socket> sock );

private:
   std::shared_ptr<AsioService> m_pService;
   std::shared_ptr<asio::io_service::work> m_pWork;
   std::shared_ptr<asio::ip::tcp::acceptor> m_pAcceptSock;
   std::shared_ptr<AsioConnectionManager> m_pConnectionsManager;
   std::shared_ptr<IAsioConnectionFactory> m_pFactory;
   std::shared_ptr<asio::ip::tcp::socket> m_pPendingSocket;

   std::shared_ptr<ServerIOStream> m_pIOStream;
};

