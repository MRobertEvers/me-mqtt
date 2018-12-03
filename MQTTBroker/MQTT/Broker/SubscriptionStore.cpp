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


SubscriptionStore::SubscriptionStore(
   std::weak_ptr<SubscriptionManager> apManager )
   : m_pManager(apManager)
{
   m_pNewRoot = std::make_shared<SubscriptionTreeNode>( 0, this );
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
      m_mapFastSubLookup.emplace( apszFilter, pNewSub->Value() );
      return pNewSub->Value();
   }
}

void
SubscriptionStore::Unsubscribe( 
   me::pcstring apszFilter, std::shared_ptr<ClientState> apszClientName )
{
   // Fast lookup
   auto iter_sub = m_mapFastSubLookup.find( apszFilter );
   if( iter_sub != m_mapFastSubLookup.end() && 
      iter_sub->second->GetSubscriberCount() > 1 )
   {
      iter_sub->second->ReleaseClient( apszClientName );
   }
   else
   {
      auto pNode = m_pNewRoot->FindNode( apszFilter );
      if( pNode )
      {
         pNode->Value()->ReleaseClient( apszClientName );
         if( pNode->Value()->GetSubscriberCount() == 0 && 
            pNode->GetChildCount() == 0 )
         {
            m_pNewRoot->DeleteNode( apszFilter );
         }
      }
      if( iter_sub != m_mapFastSubLookup.end() )
      {
         m_mapFastSubLookup.erase( iter_sub );
      }
   }
}

std::vector<std::shared_ptr<Subscription>> 
SubscriptionStore::GetSubscriptions( Topic apszFilter )
{
   std::vector<std::shared_ptr<Subscription>> vecSubs;
   auto pCurStk = new std::stack<std::shared_ptr<SubscriptionTreeNode>>();
   auto pNextStk = new std::stack<std::shared_ptr<SubscriptionTreeNode>>();
   std::shared_ptr<SubscriptionTreeNode> pNode;
   me::utils::pcview szLevel;

   pNextStk->push( m_pNewRoot );
  
   size_t iLevels = apszFilter.Levels();
   size_t iCurLevel = 0;
   while( !pCurStk->empty() || !pNextStk->empty() )
   {
      if( pCurStk->empty() )
      {
         auto tmp = pCurStk;
         pCurStk = pNextStk;
         pNextStk = tmp;

         szLevel = apszFilter.PeekLevel( iCurLevel + 1 );
         pNode = pCurStk->top();
         pCurStk->pop();

         iCurLevel++;
      }
   
      // Find matches for this level;
      auto match_node = pNode->FindDirectChild( "#" );
      if( match_node && match_node->Value() )
      {
         vecSubs.push_back( match_node->Value() );
      }
   
      match_node = pNode->FindDirectChild( "+" );
      if( match_node )
      {
         if( iCurLevel == iLevels && match_node->Value() )
         {
            vecSubs.push_back( match_node->Value() );
         }
         else if( iCurLevel < iLevels )
         {
            pNextStk->push( match_node );
         }
      }

      match_node = pNode->FindDirectChild( szLevel );
      if( match_node )
      {
         if( iCurLevel == iLevels && match_node->Value() )
         {
            vecSubs.push_back( match_node->Value() );
         }
         else if( iCurLevel < iLevels )
         {
            pNextStk->push( match_node );
         }
      }
   }

   delete pCurStk;
   delete pNextStk;

   return vecSubs;
}

void 
SubscriptionStore::RemoveSubscription( me::pcstring apszFilter )
{
   m_pNewRoot->DeleteNode( apszFilter );
}

std::shared_ptr<Subscription> 
SubscriptionStore::Create( Topic apTopic )
{
   return std::make_shared<Subscription>(
      apTopic.GetFilter(), m_pManager.lock() );
}


}

