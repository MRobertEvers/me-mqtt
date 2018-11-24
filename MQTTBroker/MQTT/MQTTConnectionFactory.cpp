#include "stdafx.h"
#include "MQTTConnectionFactory.h"
#include "MQTTConnection.h"

MQTTConnectionFactory::MQTTConnectionFactory( std::shared_ptr<ServerIOStream> aIOStream )
   : m_pIOStream(aIOStream)
{
}

MQTTConnectionFactory::~MQTTConnectionFactory()
{
}

std::shared_ptr<AsioConnection>
MQTTConnectionFactory::NewConnection( std::shared_ptr<asio::ip::tcp::socket> sock )
{
   return std::shared_ptr<AsioConnection>( new MQTTConnection( sock, m_pIOStream ) );
}
