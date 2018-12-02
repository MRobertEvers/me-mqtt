#include "stdafx.h"
#include "Topic.h"
#include "RetainedTopicStore.h"
#include "ClientState.h"
#include "Subscription.h"
#include "Topic.h"
#include "SubscriptionManager.h"
#include "RetainedTopic.h"
#include <algorithm>
#include <stack>

namespace me
{

RetainedTopicStore::RetainedTopicStore(  )
{
   m_pNewRoot = std::make_shared<RetainTreeNode>( 0, this );
}

RetainedTopicStore::~RetainedTopicStore()
{
}

std::shared_ptr<RetainedTopic> 
RetainedTopicStore::Retain( std::shared_ptr<ApplicationMessage> apMsg )
{
   auto pNewSub = m_pNewRoot->AddNode( apMsg->GetTopic() );
   return pNewSub->Value();
}

std::vector<std::shared_ptr<RetainedTopic>> 
RetainedTopicStore::GetRetainedMessages( Topic apszFilter )
{
   std::vector<std::shared_ptr<RetainedTopic>> vecTopics;
   auto pCurStk = new std::stack<std::shared_ptr<RetainTreeNode>>();
   auto pNextStk = new std::stack<std::shared_ptr<RetainTreeNode>>();
   std::shared_ptr<RetainTreeNode> pNode;
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

      if( szLevel == "#" )
      {
         if( pNode->Value() )
         {
            vecTopics.push_back( pNode->Value() );
         }

         // Entire Sub Tree
         for( auto node : pNode->BFT() )
         {
            if( node->Value() )
            {
               vecTopics.push_back( node->Value() );
            }
         }
      }
      else if( szLevel == "+" )
      {
         // Just the children
         if( iCurLevel < iLevels )
         {
            for( auto node : pNode->BFT( 1 ) )
            {
               pNextStk->push( node );
            }
         }
         else
         {
            if( pNode->Value() )
            {
               vecTopics.push_back( pNode->Value() );
            }

            for( auto node : pNode->BFT( 1 ) )
            {
               if( node->Value() )
               {
                  vecTopics.push_back( node->Value() );
               }
            }
         }
      }
      else
      {
         // Find a child that matches.
         auto match_node = pNode->FindDirectChild( szLevel );
         if( match_node )
         {
            if( iCurLevel == iLevels && match_node->Value() )
            {
               vecTopics.push_back( match_node->Value() );
            }
            else if( iCurLevel < iLevels )
            {
               pNextStk->push( match_node );
            }
         }
      }
   }

   delete pCurStk;
   delete pNextStk;

   return vecTopics;
}

void 
RetainedTopicStore::ReleaseRetainedMessage( Topic apszFilter )
{
   m_pNewRoot->DeleteNode( apszFilter );
}

std::shared_ptr<RetainedTopic>
RetainedTopicStore::Create( Topic aTopic )
{
   return std::make_shared<RetainedTopic>( aTopic );
}

}

