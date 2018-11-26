#pragma once
#include "Definitions.h"
#include "ClientStateLedger.h"
#include "Utils.h"
#include <string>
#include <map>
namespace me
{
class Subscription;
class ClientState;
class SubscriptionManager : public std::enable_shared_from_this<SubscriptionManager>
{
public:
   SubscriptionManager();
   ~SubscriptionManager();

   void Subscribe( std::shared_ptr<ClientState> apszClientName, me::pcstring apszTopicFilter );
   void Unsubscribe( std::shared_ptr<ClientState> apszClientName, me::pcstring apszTopicFilter );
   std::vector<std::shared_ptr<Subscription>> GetSubscriptions( me::pcstring apszTopicName );

   void ReleaseSubscription( me::pcstring apszTopicFilter );

private:
   std::map<me::pcstring, std::weak_ptr<Subscription>, me::utils::pcstringless> m_mapFilters;
};

}