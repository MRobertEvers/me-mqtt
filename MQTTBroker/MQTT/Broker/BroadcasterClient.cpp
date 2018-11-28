#include "stdafx.h"
#include "BroadcasterClient.h"
#include "Broadcaster.h"
#include "BrokerClient.h"
#include "ClientState.h"
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
   // If the client disconnects and the clean session flag was set, then
   // the session needs to be released.
   m_pState->UnsubscribeAll();
}

void
BroadcasterClient::ConnectClient( std::weak_ptr<BrokerClient> apClient )
{
   auto pClient = apClient.lock();
   if( pClient )
   {
      m_pClient = apClient;
      m_pszClientName = pClient->GetClientName();
      m_pState = m_pBroadcaster->ConnectClient( shared_from_this() );
   }
}

void
BroadcasterClient::DisconnectClient( bool abSaveState )
{
   if( !abSaveState )
   {
      m_pBroadcaster->DisconnectClient( shared_from_this() );
   }
}

me::pcstring
BroadcasterClient::GetClientName() const
{
   return m_pszClientName;
}

std::weak_ptr<BrokerClient>
BroadcasterClient::GetClient() const
{
   return m_pClient;
}

std::shared_ptr<ClientState> 
BroadcasterClient::GetState()
{
   return m_pState;
}

void 
BroadcasterClient::BroadcastPublishMessage( 
   std::shared_ptr<ApplicationMessage> apMsg ) const
{
   m_pBroadcaster->BroadcastMessage( apMsg );
}

void
BroadcasterClient::SubscribeToTopics( 
   unsigned short aiRequestIds,
   std::vector<SubscribeRequest> avecTopics ) const
{
   m_pBroadcaster->Subscribe( m_pState, aiRequestIds, avecTopics );
}

void 
BroadcasterClient::UnsubscribeFromTopics(
   unsigned short aiRequestIds,
   std::vector<me::pcstring> apszTopicFilter ) const
{
   m_pBroadcaster->Unsubscribe( m_pState, aiRequestIds, apszTopicFilter );
}

}