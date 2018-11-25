#include "stdafx.h"
#include "BroadcasterClient.h"
#include "Broadcaster.h"
#include "BrokerClient.h"
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
BroadcasterClient::ConnectClient( std::weak_ptr<BrokerClient> apClient )
{
   auto pClient = apClient.lock();
   if( pClient )
   {
      m_pClient = apClient;
      m_pBroadcaster->ConnectClient( shared_from_this() );
      m_szClientName = pClient->GetClientName();
   }
}

me::pcstring 
BroadcasterClient::GetClientName() const
{
   return m_szClientName;
}

std::weak_ptr<BrokerClient>
BroadcasterClient::GetClient() const
{
   return m_pClient;
}

void 
BroadcasterClient::BroadcastPublishMessage( 
   me::pcstring apszTopic, me::pcstring apszPayload, 
   unsigned char aiQOS, bool abRetain ) const
{
   auto pMessage = std::make_shared<ApplicationMessage>( 
      apszTopic, apszPayload, aiQOS, abRetain 
      );

   m_pBroadcaster->BroadcastMessage( pMessage );
}

void
BroadcasterClient::SubscribeToTopic( me::pcstring apszTopicFilter ) const
{

}

}