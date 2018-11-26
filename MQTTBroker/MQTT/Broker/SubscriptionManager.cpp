#include "stdafx.h"
#include "SubscriptionManager.h"
#include "ClientState.h"
#include "Subscription.h"

namespace me
{
SubscriptionManager::SubscriptionManager()
{

}

SubscriptionManager::~SubscriptionManager()
{

}
void
SubscriptionManager::Subscribe( std::shared_ptr<ClientState> apszClientName, me::pcstring apszTopicFilter )
{
   std::shared_ptr<Subscription> pSub;
   auto iter_sub = m_mapFilters.find( apszTopicFilter );
   if( iter_sub != m_mapFilters.end() )
   {
      pSub = iter_sub->second.lock();
   }
   
   if( !pSub )
   {
      pSub = std::make_shared<Subscription>( apszTopicFilter, shared_from_this() );
      m_mapFilters.emplace( apszTopicFilter, pSub );
   }

   apszClientName->Subscribe( pSub );
}

void 
SubscriptionManager::Unsubscribe( std::shared_ptr<ClientState> apszClientName, me::pcstring apszTopicFilter )
{
   std::shared_ptr<Subscription> pSub;
   auto iter_sub = m_mapFilters.find( apszTopicFilter );
   if( iter_sub != m_mapFilters.end() )
   {
      pSub = iter_sub->second.lock();
   }

   if( pSub )
   {
      pSub->ReleaseClient( apszClientName );
   }
}

std::vector<std::shared_ptr<Subscription>>
SubscriptionManager::GetSubscriptions( me::pcstring apszTopicName )
{
   std::vector<std::shared_ptr<Subscription>> m_vecSubs;
   for( auto filter : m_mapFilters )
   {
      // Check if filter match.
      m_vecSubs.push_back( filter.second.lock() );
   }

   return m_vecSubs;
}
void 
SubscriptionManager::ReleaseSubscription( me::pcstring apszTopicFilter )
{
   // TODO: Check that the subscription is empty.
   std::shared_ptr<Subscription> pSub;
   auto iter_sub = m_mapFilters.find( apszTopicFilter );
   if( iter_sub != m_mapFilters.end() )
   {
      m_mapFilters.erase( iter_sub );
   }
}
}
