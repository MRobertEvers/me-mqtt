#pragma once
#include "Definitions.h"
#include "Utils.h"
#include "Topic.h"
#include "TPathTree.h"
#include <string>

namespace me
{

class SubscriptionManager;
class Subscription;
class ClientState;

class SubscriptionStore;
typedef Node<
   std::shared_ptr<Subscription>,
   Topic,
   SubscriptionStore*
> SubscriptionTreeNode;

class SubscriptionManager;
class SubscriptionStore
{
public:
   SubscriptionStore( std::weak_ptr<SubscriptionManager> apManager );
   ~SubscriptionStore();

   // Filter needs to match exactly.
   std::shared_ptr<Subscription> Subscribe( me::pcstring apszFilter );
   void Unsubscribe( 
      me::pcstring apszFilter, 
      std::shared_ptr<ClientState> apszClientName );

   std::vector<std::shared_ptr<Subscription>> GetSubscriptions( 
      Topic apszFilter );

   void RemoveSubscription( me::pcstring apszFilter );

   std::shared_ptr<Subscription> Create( Topic apTopic );

private:
   std::shared_ptr<SubscriptionTreeNode> m_pNewRoot;
   std::weak_ptr<SubscriptionManager> m_pManager;

   std::map<
      me::pcstring,
      std::shared_ptr<Subscription>,
      me::utils::pcstringless
   > m_mapFastSubLookup;
};

}