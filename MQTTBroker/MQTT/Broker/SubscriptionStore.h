#pragma once
#include "Definitions.h"
#include <string>

namespace me
{

class pcview
{
public:
   pcview();
   pcview( char const* aszBase );
   pcview( me::pcstring apszSource, char const* aszStart, size_t aiLen );
   ~pcview();

   size_t size() const;
   char const* data() const;

   bool operator==( const pcview& rhs ) const;
   bool operator==( const std::string& rhs ) const;

private:
   me::pcstring m_szSource;
   char const* m_szStart;
   size_t m_iLen;
};

struct pcviewless
{
public:
   bool operator()( const me::pcview& lhs, const me::pcview& rhs ) const;
};

class Filter
{
public:
   Filter( me::pcstring apszSource );
   ~Filter();
   me::pcstring GetFilter() const;
   pcview PeekLevel( unsigned int aiLevel ) const;
   size_t Levels() const;
private:
   me::pcstring m_pszFilter;
};

class SubscriptionManager;
class Subscription;
class ClientState;
class MatchNode
{
public:
   MatchNode( unsigned int aiLevel, me::pcstring apszFilter, std::weak_ptr<SubscriptionManager> apManager );
   ~MatchNode();
   std::vector<std::shared_ptr<Subscription>> FindSubscriptions( Filter apszTopicName );
   std::shared_ptr<Subscription> AddSubscription( Filter apszFilter, std::shared_ptr<ClientState> apszClientName );
   bool RemoveSubscriber( Filter apszTopicName, std::shared_ptr<ClientState> apszClientName );

   pcview PeekLevel( unsigned int aiLevel, me::pcstring apszFilter ) const;
private:


   unsigned int m_iLevel;

   pcview m_pszLevel;
   std::shared_ptr<Subscription> m_wpSub;
   std::weak_ptr<SubscriptionManager> m_pManager;
   std::map<pcview, std::shared_ptr<MatchNode>, pcviewless> m_mapChildren;
};


class SubscriptionManager;
class SubscriptionStore
{
public:
   SubscriptionStore( std::weak_ptr<SubscriptionManager> apManager );
   ~SubscriptionStore();

   // Filter needs to match exactly.
   std::shared_ptr<Subscription>  Subscribe( me::pcstring apszFilter, std::shared_ptr<ClientState> awpSub );
   void Unsubscribe( me::pcstring apszFilter, std::shared_ptr<ClientState> apszClientName );
   std::shared_ptr<Subscription> GetSubscription( me::pcstring apszFilter );
   std::vector<std::shared_ptr<Subscription>> GetSubscriptions( me::pcstring apszFilter );
   void RemoveSubscription( me::pcstring apszFilter );

private:
   std::shared_ptr<MatchNode> m_pRoot;
   std::weak_ptr<SubscriptionManager> m_pManager;
};

}