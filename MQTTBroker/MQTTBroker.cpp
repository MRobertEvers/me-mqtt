// MQTTBroker.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "AsioService.h"
#include "AsioServer.h"
#include "ServerIOStream.h"
#include "MQTTConnectionFactory.h"
#include "Broker\Broadcaster.h"
#include <memory>


using namespace me;
int main()
{
   std::shared_ptr<AsioService> pService( new AsioService( 4 ) );

   std::shared_ptr<ServerIOStream> pIOStream( new ServerIOStream() );
   std::shared_ptr<Broadcaster> pBroadcaster( new Broadcaster( pService ) );
   std::shared_ptr<IAsioConnectionFactory> pFactory( new MQTTConnectionFactory( pIOStream, pBroadcaster ) );
   std::shared_ptr<AsioServer> pServer( new AsioServer( pService, pFactory, pIOStream ) );

   pServer->StartListener("127.0.0.1", "8000");

   std::cin.get();

   return 0;
}