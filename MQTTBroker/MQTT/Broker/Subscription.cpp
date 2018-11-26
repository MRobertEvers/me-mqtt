#include "stdafx.h"
#include "Subscription.h"
#include "ClientState.h"
#include "SubscriptionManager.h"
#include <algorithm>

namespace me
{

Subscription::Subscription( 
   me::pcstring apszFilter,
   std::weak_ptr<SubscriptionManager> apManager )
   : m_pManager(apManager), m_szTopicFilter(apszFilter)
{
}

Subscription::~Subscription()
{
   // Remove from manager. // TODO:
   m_pManager.lock()->ReleaseSubscription( m_szTopicFilter );
}

void 
Subscription::RecordClient( std::shared_ptr<ClientState> apszClient )
{
   m_setClients.insert( apszClient );
}

void
Subscription::ReleaseClient( std::shared_ptr<ClientState> apszClient )
{
   m_setClients.erase( apszClient );
}

std::set<std::shared_ptr<ClientState>>
Subscription::GetSubscribers()
{
   return m_setClients;
}



}
