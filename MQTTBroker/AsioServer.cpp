#include "stdafx.h"
#include "AsioServer.h"
#include "MQTTConnection.h"


AsioServer::AsioServer( std::shared_ptr<AsioService> apIOService,
                        std::shared_ptr<ServerIOStream> apIOStream )
   : 
      m_pService( apIOService ),
      m_pWork( 
         std::shared_ptr<asio::io_service::work>( new asio::io_context::work( *apIOService->GetService() ) )
      ), // As long as this server is running, the service wont end..
      m_pIOStream(apIOStream),
      m_pAcceptSock( new asio::ip::tcp::acceptor( *apIOService->GetService() ) ),
      m_pConnectionsManager( new AsioConnectionManager() )
{
   
}


AsioServer::~AsioServer()
{

}


void 
AsioServer::StartListener( const std::string& aszHost, const std::string& aszPort )
{
   try
   {
      asio::ip::tcp::resolver resolver( *m_pService->GetService() );
      asio::ip::tcp::resolver::query query(
         aszHost,
         aszPort
      );
      asio::ip::tcp::endpoint endpoint = *resolver.resolve( query );
      m_pAcceptSock->open( endpoint.protocol() );
      m_pAcceptSock->set_option( asio::ip::tcp::acceptor::reuse_address( false ) );
      m_pAcceptSock->bind( endpoint );
      m_pAcceptSock->listen( asio::socket_base::max_connections );
   
      *m_pIOStream << "Listening on: " << endpoint << std::endl;

      awaitConnection();
   }
   catch( std::exception & ex )
   {
      *m_pIOStream << "[" << std::this_thread::get_id()
         << "] Exception: " << ex.what() << std::endl;
   }
}

void AsioServer::awaitConnection()
{
   auto sock = getNextSocket();
   auto conn_handler = std::bind( 
      &AsioServer::handleConnect, this, std::placeholders::_1, sock 
   );
   m_pAcceptSock->async_accept( *sock, conn_handler );
}

std::shared_ptr<asio::ip::tcp::socket>
AsioServer::getNextSocket()
{
   auto pSock = std::shared_ptr<asio::ip::tcp::socket>(
      new asio::ip::tcp::socket( *m_pService->GetService() ) 
      );
   m_vecSocks.push_back( pSock );
   return pSock;
}

void
AsioServer::handleConnect( const asio::error_code & ec, std::shared_ptr<asio::ip::tcp::socket> sock )
{
   awaitConnection();

   if( ec )
   {
      *m_pIOStream << "[" << std::this_thread::get_id()
         << "] Error: " << ec << std::endl;
   }
   else
   {
      *m_pIOStream << "[" << std::this_thread::get_id()
         << "] Accepted!" << std::endl;

      m_pConnectionsManager->AddConnection(
         std::shared_ptr<AsioConnection>( new MQTTConnection( sock, m_pIOStream ) )
      );
   }
}

