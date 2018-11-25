#include "stdafx.h"
#include "BroadcasterClient.h"
#include "Broadcaster.h"
#include "ApplicationMessage.h"

namespace me
{
BroadcasterClient::BroadcasterClient( 
   std::shared_ptr<Broadcaster> apBroadcaster )
   : m_pBroadcaster( apBroadcaster )
{
}

BroadcasterClient::~BroadcasterClient()
{

}

void
BroadcasterClient::Connect( std::weak_ptr<BrokerClient> apClient )
{
   m_pClient = apClient;
}

std::weak_ptr<BrokerClient>
BroadcasterClient::GetClient()
{
   return m_pClient;
}

void 
BroadcasterClient::BroadcastPublishMessage( 
   me::pcstring apszTopic, me::pcstring apszPayload, 
   unsigned char aiQOS, bool abRetain )
{
   auto pMessage = std::make_shared<ApplicationMessage>( 
      apszTopic, apszPayload, aiQOS, abRetain 
      );

   m_pBroadcaster->BroadcastMessage( pMessage );
}

}