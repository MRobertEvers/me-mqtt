#include "stdafx.h"
#include "SubscriptionManager.h"
#include "ClientState.h"
#include "Subscription.h"
#include "SubscriptionStore.h"

namespace me
{
SubscriptionManager::SubscriptionManager()
{
}

SubscriptionManager::~SubscriptionManager()
{

}
void
SubscriptionManager::Subscribe( 
   std::shared_ptr<ClientState> apszClientName,
   unsigned short aiRequestId,
   std::vector<SubscribeRequest> avecTopics )
{
   if( !m_pSubStore )
   {
      m_pSubStore = std::make_shared<SubscriptionStore>( shared_from_this() );
   }
   std::vector<unsigned char> vecResponses;
   for( auto topic : avecTopics )
   {
      auto pSub = m_pSubStore->Subscribe( topic.Topic );
      vecResponses.push_back( topic.QOS );
      apszClientName->Subscribe( pSub, topic.QOS );
   }

   apszClientName->NotifySubscribe( aiRequestId, vecResponses );
}

void 
SubscriptionManager::Unsubscribe( 
   std::shared_ptr<ClientState> apszClientName,
   unsigned short aiRequestId,
   std::vector<me::pcstring> avecTopics )
{
   for( auto topic : avecTopics )
   {
      m_pSubStore->Unsubscribe( topic, apszClientName );
   }
   apszClientName->NotifyUnubscribe( aiRequestId );
}

std::vector<std::shared_ptr<Subscription>>
SubscriptionManager::GetSubscriptions( me::pcstring apszTopicName )
{
   if( !m_pSubStore )
   {
      m_pSubStore = std::make_shared<SubscriptionStore>( shared_from_this() );
   }
   return m_pSubStore->GetSubscriptions( apszTopicName );
}
void 
SubscriptionManager::ReleaseSubscription( me::pcstring apszTopicFilter )
{
   // TODO: Check that the subscription is empty.
   std::shared_ptr<Subscription> pSub;
   auto iter_sub = m_mapFilters.find( apszTopicFilter );
   if( iter_sub != m_mapFilters.end() )
   {
      m_pSubStore->RemoveSubscription( apszTopicFilter );

      m_mapFilters.erase( iter_sub );
   }
}
}
