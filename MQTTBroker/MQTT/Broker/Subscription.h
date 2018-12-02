#pragma once
#include "Definitions.h"
#include <string>
#include <map>

namespace me
{
class SubscriptionManager;
class ClientState;
// Tracks all the clients subsribed to this filter.
class Subscription
{
public:
   // Weak pointer to the manager so a subscription can release itself
   Subscription( me::pcstring apszFilter, std::weak_ptr<SubscriptionManager> apManager );
   ~Subscription();

   void RecordClient( std::shared_ptr<ClientState> apszClient, unsigned char maxQOS );
   void ReleaseClient( std::shared_ptr<ClientState> apszClient );
   std::map<std::shared_ptr<ClientState>, unsigned char> GetSubscribers();
   size_t GetSubscriberCount() const;

private:
   me::pcstring m_szTopicFilter;
   std::weak_ptr<SubscriptionManager> m_pManager;
   std::map<std::shared_ptr<ClientState>, unsigned char> m_setClients;
};

}