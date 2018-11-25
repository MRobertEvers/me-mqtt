#include "stdafx.h"
#include "MQTTConnectionFactory.h"
#include "MQTTConnection.h"
#include "Asio\AsioService.h"
#include "Broker\Broadcaster.h"


namespace me
{
MQTTConnectionFactory::MQTTConnectionFactory( 
   std::shared_ptr<ServerIOStream> aIOStream,
   std::shared_ptr<Broadcaster> apBroadcaster )
   : m_pIOStream( aIOStream ), m_pBroadcaster(apBroadcaster)
{
}

MQTTConnectionFactory::~MQTTConnectionFactory()
{
}

std::shared_ptr<AsioConnection>
MQTTConnectionFactory::NewConnection( std::shared_ptr<asio::ip::tcp::socket> sock )
{
   return std::shared_ptr<AsioConnection>( new MQTTConnection( sock, m_pBroadcaster, m_pIOStream ) );
}
}