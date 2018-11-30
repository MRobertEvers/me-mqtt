#include "stdafx.h"
#include "Topic.h"
#include "SubscriptionStore.h"
#include "ClientState.h"
#include "Subscription.h"
#include "SubscriptionManager.h"
#include <algorithm>
#include <stack>

namespace me
{
bool
pcviewless::operator()( const me::pcview& lhs, const me::pcview& rhs ) const
{
   bool res = strncmp( lhs.data(), rhs.data(), lhs.size() ) < 0;
   return res;
}

SubscriptionStore::SubscriptionStore( std::weak_ptr<SubscriptionManager> apManager )
   : m_pManager(apManager)
{
   m_pRoot = std::make_shared<MatchNode>(
      0,
      std::make_shared<std::string>( "" ),
      m_pManager
      );
}

SubscriptionStore::~SubscriptionStore()
{
}

std::shared_ptr<Subscription>
SubscriptionStore::Subscribe( me::pcstring apszFilter, std::shared_ptr<ClientState>  awpSub )
{
   return m_pRoot->AddSubscription( apszFilter, awpSub );
}

void
SubscriptionStore::Unsubscribe( 
   me::pcstring apszFilter, std::shared_ptr<ClientState> apszClientName )
{
   m_pRoot->RemoveSubscriber( apszFilter, apszClientName );
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

MatchNode::MatchNode( 
   unsigned int aiLevel, me::pcstring apszFilter, std::weak_ptr<SubscriptionManager> apManager )
   : m_pManager(apManager), m_iLevel(aiLevel)
{
}

MatchNode::~MatchNode()
{

}

std::vector<std::shared_ptr<Subscription>> 
MatchNode::FindSubscriptions( Filter apszTopicName )
{
   std::vector<std::shared_ptr<Subscription>> m_vecSubs;
   std::map<pcview, std::shared_ptr<MatchNode>, pcviewless>* mapCurLevel = &m_mapChildren;
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
         else if( iCurLevel+1 < iLevels )
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
   Filter apszFilter, std::shared_ptr<ClientState> apszClientName )
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
      return iter_exists->second->AddSubscription( apszFilter, apszClientName );
   }
   else
   {
      auto item = m_mapChildren.emplace( next, std::make_shared<MatchNode>( m_iLevel + 1, apszFilter.GetFilter(), m_pManager ) );
      return item.first->second->AddSubscription( apszFilter, apszClientName );
   }
}

bool 
MatchNode::RemoveSubscriber(
   Filter apszTopicName, std::shared_ptr<ClientState> apszClientName )
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

pcview 
MatchNode::PeekLevel( unsigned int aiLevel, me::pcstring apszFilter ) const
{

   std::string const& szTopic = *apszFilter;

   size_t i = 0;
   while( i  )

   return pcview();
}

pcview::pcview()

{
   m_szSource = (std::make_shared<std::string>());
   m_szStart = m_szSource->data();
   m_iLen = (0);
}

pcview::pcview( char const * aszBase )
{
   m_szStart = ( aszBase );
   m_iLen = strlen( aszBase );
}

pcview::pcview( me::pcstring apszSource, char const* aszStart, size_t aiLen )
   : m_szSource(apszSource), m_szStart(aszStart), m_iLen(aiLen)
{

}

pcview::~pcview()
{

}

size_t
pcview::size() const
{
   return m_iLen;
}

char const*
pcview::data() const
{
   return m_szStart;
}

bool 
pcview::operator==( const pcview& rhs ) const
{
   return memcmp( m_szStart, rhs.m_szStart, m_iLen );
}

bool 
pcview::operator==( const std::string& rhs ) const
{
   return memcmp( m_szStart, rhs.data(), m_iLen );
}
Filter::Filter( me::pcstring apszSource )
   : m_pszFilter(apszSource)
{
}
Filter::~Filter()
{
}

me::pcstring Filter::GetFilter() const
{
   return m_pszFilter;
}


pcview
Filter::PeekLevel( unsigned int aiLevel ) const
{
   if( aiLevel == 0 || aiLevel > Levels() )
   {
      return pcview();
   }
   int i = 0;
   int iOff = 0;
   int iEnd = -1;
   while( i < aiLevel )
   {
      size_t iNextOff = m_pszFilter->find( '/', iEnd + 1 );
      if( iNextOff != std::string::npos )
      {
         iOff = iEnd + 1;
         iEnd = iNextOff;
      }
      else
      {
         iOff = iEnd + 1;
         iEnd = m_pszFilter->size();
      }
      i++;
   }

   return pcview( m_pszFilter, m_pszFilter->data()+iOff, iEnd - iOff );
}
size_t 
Filter::Levels() const
{
   return 1 + std::count( m_pszFilter->begin(), m_pszFilter->end(), '/' );
}
}

