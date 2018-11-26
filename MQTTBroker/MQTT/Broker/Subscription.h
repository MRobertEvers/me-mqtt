#pragma once
#include "Definitions.h"
#include <string>
#include <set>

namespace me
{
class SubscriptionManager;
class ClientState;
// Tracks all the clients subsribed to this filter.
class Subscription
{
public:
   Subscription( me::pcstring apszFilter, std::weak_ptr<SubscriptionManager> apManager );
   ~Subscription();

   void RecordClient( std::shared_ptr<ClientState> apszClient );
   void ReleaseClient( std::shared_ptr<ClientState> apszClient );
   std::set<std::shared_ptr<ClientState>> GetSubscribers();

private:
   me::pcstring m_szTopicFilter;
   std::weak_ptr<SubscriptionManager> m_pManager;
   std::set<std::shared_ptr<ClientState>> m_setClients;
};

}