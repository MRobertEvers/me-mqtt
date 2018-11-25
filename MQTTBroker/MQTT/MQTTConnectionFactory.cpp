#include "stdafx.h"
#include "MQTTConnectionFactory.h"
#include "MQTTConnection.h"
#include "Asio\AsioService.h"
#include "Broker\Broadcaster.h"
namespace me
{
MQTTConnectionFactory::MQTTConnectionFactory( std::shared_ptr<ServerIOStream> aIOStream, std::shared_ptr<AsioService> apIOService )
   : m_pIOStream( aIOStream )
{
   m_pBroadcaster = std::make_shared<Broadcaster>( apIOService );
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