#include "stdafx.h"
#include "Topic.h"
#include "SubscriptionStore.h"
#include "ClientState.h"
#include "Subscription.h"
#include "Topic.h"
#include "SubscriptionManager.h"
#include <algorithm>
#include <stack>

namespace me
{


SubscriptionStore::SubscriptionStore( std::weak_ptr<SubscriptionManager> apManager )
   : m_pManager(apManager)
{
   m_pRoot = std::make_shared<MatchNode>(
      0,
      std::make_shared<std::string>( "" ),
      m_pManager
      );
   m_pNewRoot = std::make_shared<SubStore>( 0, this );
}

SubscriptionStore::~SubscriptionStore()
{
}

std::shared_ptr<Subscription>
SubscriptionStore::Subscribe( me::pcstring apszFilter )
{
   // Fast lookup
   auto iter_sub = m_mapFastSubLookup.find( apszFilter );
   if( iter_sub != m_mapFastSubLookup.end() )
   {
      return iter_sub->second;
   }
   else
   {
      auto pNewSub = m_pNewRoot->AddNode( apszFilter );
      m_mapFastSubLookup.emplace( apszFilter, pNewSub );
      return pNewSub;
      // return m_pRoot->AddSubscription( apszFilter );
   }
}

void
SubscriptionStore::Unsubscribe( 
   me::pcstring apszFilter, std::shared_ptr<ClientState> apszClientName )
{
   // Fast lookup
   auto iter_sub = m_mapFastSubLookup.find( apszFilter );
   if( iter_sub != m_mapFastSubLookup.end() && iter_sub->second->GetSubscribers().size() > 1 )
   {
      iter_sub->second->ReleaseClient( apszClientName );
   }
   else
   {
      m_pRoot->RemoveSubscriber( apszFilter, apszClientName );
   }
}

std::shared_ptr<Subscription>
SubscriptionStore::GetSubscription( me::pcstring apszFilter )
{
   return std::shared_ptr<Subscription>();
}

std::vector<std::shared_ptr<Subscription>> 
SubscriptionStore::GetSubscriptions( me::pcstring apszFilter )
{
   return m_pRoot->FindSubscriptions(apszFilter);
}

void 
SubscriptionStore::RemoveSubscription( me::pcstring apszFilter )
{

}

std::shared_ptr<Subscription> 
SubscriptionStore::Create( Topic apTopic )
{
   return std::make_shared<Subscription>( apTopic.GetFilter(), m_pManager.lock() );
}

MatchNode::MatchNode( 
   unsigned int aiLevel, me::pcstring apszFilter, std::weak_ptr<SubscriptionManager> apManager )
   : m_pManager(apManager), m_iLevel(aiLevel)
{
}

MatchNode::~MatchNode()
{

}

std::vector<std::shared_ptr<Subscription>> 
MatchNode::FindSubscriptions( Topic apszTopicName )
{
   std::vector<std::shared_ptr<Subscription>> m_vecSubs;
   std::map<utils::pcview, std::shared_ptr<MatchNode>, utils::pcviewless>* mapCurLevel = &m_mapChildren;
   std::stack<MatchNode*> stk;

   stk.push( this );

   size_t iLevels = apszTopicName.Levels();
   size_t iCurLevel = m_iLevel;
   while( !stk.empty() )
   {
      auto szLevel = apszTopicName.PeekLevel( iCurLevel + 1 );
      auto pNode = stk.top();
      stk.pop();
      mapCurLevel = &pNode->m_mapChildren;

      // Find matches for this level;
      auto iter_find_match = mapCurLevel->find( "#" );
      if( iter_find_match != mapCurLevel->end() && iter_find_match->second->m_wpSub )
      {
         m_vecSubs.push_back( iter_find_match->second->m_wpSub );
      }

      iter_find_match = mapCurLevel->find( "+" );
      if( iter_find_match != mapCurLevel->end() )
      {
         if( iCurLevel + 1 == iLevels && iter_find_match->second->m_wpSub )
         {
            m_vecSubs.push_back( iter_find_match->second->m_wpSub );
         }
         else if( iCurLevel + 1 < iLevels )
         {
            stk.push( &*iter_find_match->second );
         }
      }

      iter_find_match = mapCurLevel->find( szLevel );
      if( iter_find_match != mapCurLevel->end() )
      {
         if( iCurLevel + 1 == iLevels && iter_find_match->second->m_wpSub )
         {
            m_vecSubs.push_back( iter_find_match->second->m_wpSub );
         }
         else if( iCurLevel + 1 < iLevels )
         {
            stk.push( &*iter_find_match->second );
         }
      }

      iCurLevel++;
   }

   return m_vecSubs;
}

std::shared_ptr<Subscription>
MatchNode::AddSubscription( 
   Topic apszFilter )
{

   if( apszFilter.Levels() == m_iLevel )
   {
      if( !m_wpSub )
      {
         m_wpSub = std::make_shared<Subscription>(apszFilter.GetFilter(), m_pManager.lock() );
      }
      return m_wpSub;
   }

   auto next = apszFilter.PeekLevel( m_iLevel + 1 );
   auto iter_exists = m_mapChildren.find( next );
   if( iter_exists != m_mapChildren.end() )
   {
      // TODO: Check if the filter is shorter and use the shorter filter.
      return iter_exists->second->AddSubscription( apszFilter );
   }
   else
   {
      auto item = m_mapChildren.emplace( next, std::make_shared<MatchNode>( m_iLevel + 1, apszFilter.GetFilter(), m_pManager ) );
      return item.first->second->AddSubscription( apszFilter );
   }
}

bool 
MatchNode::RemoveSubscriber(
   Topic apszTopicName, std::shared_ptr<ClientState> apszClientName )
{
   if( apszTopicName.Levels() == m_iLevel )
   {
      if( m_wpSub )
      {
         m_wpSub->ReleaseClient( apszClientName );
      }
      return true;
   }

   auto next = apszTopicName.PeekLevel( m_iLevel + 1 );
   auto iter_exists = m_mapChildren.find( next );
   if( iter_exists != m_mapChildren.end() )
   {
      auto remed = iter_exists->second->RemoveSubscriber( apszTopicName, apszClientName );
      if( remed )
      {
         bool bHasSub = iter_exists->second->m_wpSub && iter_exists->second->m_wpSub->GetSubscribers().size() == 0;
         if( bHasSub || (!iter_exists->second->m_wpSub && iter_exists->second->m_mapChildren.size()==0) )
         {
            m_mapChildren.erase( iter_exists );
         }
      }
      return remed;
   }
   else
   {
      return false;
   }
}

}

