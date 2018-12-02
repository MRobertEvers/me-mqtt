#pragma once
#include "Definitions.h"
#include "Utils.h"
#include "Topic.h"
#include "RetainedTopicStore.h"
#include <string>

namespace me
{

class SubscriptionManager;
class Subscription;
class ClientState;
class MatchNode
{
public:
   MatchNode( unsigned int aiLevel, me::pcstring apszFilter, std::weak_ptr<SubscriptionManager> apManager );
   ~MatchNode();
   std::vector<std::shared_ptr<Subscription>> FindSubscriptions( Topic apszTopicName );
   std::shared_ptr<Subscription> AddSubscription( Topic apszFilter );
   bool RemoveSubscriber( Topic apszTopicName, std::shared_ptr<ClientState> apszClientName );

private:


   unsigned int m_iLevel;

   utils::pcview m_pszLevel;
   std::shared_ptr<Subscription> m_wpSub;
   std::weak_ptr<SubscriptionManager> m_pManager;
   std::map<utils::pcview, std::shared_ptr<MatchNode>, utils::pcviewless> m_mapChildren;
};

class SubscriptionStore;
typedef Node<
   std::shared_ptr<Subscription>,
   Topic,
   SubscriptionStore*
> SubStore;

class SubscriptionManager;
class SubscriptionStore
{
public:
   SubscriptionStore( std::weak_ptr<SubscriptionManager> apManager );
   ~SubscriptionStore();

   // Filter needs to match exactly.
   std::shared_ptr<Subscription>  Subscribe( me::pcstring apszFilter );
   void Unsubscribe( me::pcstring apszFilter, std::shared_ptr<ClientState> apszClientName );
   std::shared_ptr<Subscription> GetSubscription( me::pcstring apszFilter );
   std::vector<std::shared_ptr<Subscription>> GetSubscriptions( me::pcstring apszFilter );
   void RemoveSubscription( me::pcstring apszFilter );

   std::shared_ptr<Subscription> Create( Topic apTopic );

private:
   std::shared_ptr<MatchNode> m_pRoot;
   std::shared_ptr<SubStore> m_pNewRoot;
   std::weak_ptr<SubscriptionManager> m_pManager;

   std::map<me::pcstring, std::shared_ptr<Subscription>, me::utils::pcstringless> m_mapFastSubLookup;
};

}