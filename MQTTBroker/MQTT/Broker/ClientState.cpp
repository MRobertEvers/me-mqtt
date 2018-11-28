#include "stdafx.h"
#include "ClientState.h"
#include "Subscription.h"
#include "BrokerClient.h"
#include "ClientStateLedger.h"

namespace me
{
ClientState::ClientState( me::pcstring apszClientName, std::weak_ptr<ClientStateLedger> apManager )
   : m_pManager(apManager), m_pszClientName(apszClientName)
{
}

ClientState::~ClientState()
{
}

void 
ClientState::SetWatcher( std::weak_ptr<BrokerClient> apSource )
{
   m_pSource = apSource;
}

void 
ClientState::DisconnectWatch()
{
   auto s = m_pSource.lock();
   if( s )
   {
      s->Disconnect();
   }
}

void 
ClientState::AddPendingOutbound( std::shared_ptr<ApplicationMessage> apMsg )
{
   auto s = m_pSource.lock();
   if( s )
   {
      s->PublishTo( apMsg );
   }
   else
   {
      m_qPendingOutbound.push( apMsg );
   }
}

std::shared_ptr<ApplicationMessage>
ClientState::PeekNextOutbound()
{
   return m_qPendingOutbound.front();
}

void
ClientState::ReleaseNextOutbound(  )
{
   m_qPendingOutbound.pop( );
}

void 
ClientState::AddPendingPuback( unsigned short id, std::shared_ptr<ApplicationMessage> apMsg )
{
   m_mapPendingPuback.emplace( id, apMsg );
}

std::shared_ptr<ApplicationMessage>
ClientState::ReleasePendingPuback( unsigned short aiId )
{
   auto iter_msg = m_mapPendingPuback.find( aiId );
   if( iter_msg != m_mapPendingPuback.end() )
   {
      auto pMsg = iter_msg->second;
      m_mapPendingPuback.erase( iter_msg );
      return pMsg;
   }
   else
   {
      return nullptr;
   }
}

void 
ClientState::AddPendingPubrec( unsigned short id, std::shared_ptr<ApplicationMessage> apMsg )
{
   m_mapPendingPubrec.emplace( id, apMsg );
}

std::shared_ptr<ApplicationMessage>
ClientState::ReleasePendingPubrec( unsigned short aiId )
{
   auto iter_msg = m_mapPendingPubrec.find( aiId );
   if( iter_msg != m_mapPendingPubrec.end() )
   {
      auto pMsg = iter_msg->second;
      m_mapPendingPubrec.erase( iter_msg );
      return pMsg;
   }
   else
   {
      return nullptr;
   }
}

void 
ClientState::AddPendingPubrel( unsigned short id, std::shared_ptr<ApplicationMessage> apMsg )
{
   m_mapPendingPubrel.emplace( id, apMsg );
}

std::shared_ptr<ApplicationMessage>
ClientState::ReleasePendingPubrel( unsigned short aiId )
{
   auto iter_msg = m_mapPendingPubrel.find( aiId );
   if( iter_msg != m_mapPendingPubrel.end() )
   {
      auto pMsg = iter_msg->second;
      m_mapPendingPubrel.erase( iter_msg );
      return pMsg;
   }
   else
   {
      return nullptr;
   }
}

void 
ClientState::AddPendingPubcomp( unsigned short id, std::shared_ptr<ApplicationMessage> apMsg )
{
   m_mapPendingPubcomp.emplace( id, apMsg );
}

std::shared_ptr<ApplicationMessage>
ClientState::ReleasePendingPubcomp( unsigned short aiId )
{
   auto iter_msg = m_mapPendingPubcomp.find( aiId );
   if( iter_msg != m_mapPendingPubcomp.end() )
   {
      auto pMsg = iter_msg->second;
      m_mapPendingPubcomp.erase( iter_msg );
      return pMsg;
   }
   else
   {
      return nullptr;
   }
}

void
ClientState::Subscribe( std::shared_ptr<Subscription> apSub, unsigned char maxQOS )
{
   m_setSubscriptions.insert( apSub );
   apSub->RecordClient( shared_from_this(), maxQOS );
}

void
ClientState::NotifySubscribe( unsigned short aiRequestId, std::vector<unsigned char> avecSub )
{
   auto s = m_pSource.lock();
   if( s )
   {
      s->NotifySubscribed( aiRequestId, avecSub );
   }
}

void
ClientState::Unsubscribe( std::shared_ptr<Subscription> apSub )
{
   m_setSubscriptions.erase( apSub );
   apSub->ReleaseClient( shared_from_this() );
}

void 
ClientState::NotifyUnubscribe(unsigned short aiRequestId)
{
   auto s = m_pSource.lock();
   if( s )
   {
      s->NotifyUnsubscribed( aiRequestId );
   }
}

void
ClientState::UnsubscribeAll()
{
   for( auto sub : m_setSubscriptions )
   {
      sub->ReleaseClient( shared_from_this() );
   }
   m_setSubscriptions.clear();
}

void 
ClientState::Destroy()
{
   UnsubscribeAll();

   m_pManager.lock()->DeleteClient( m_pszClientName );
}

}