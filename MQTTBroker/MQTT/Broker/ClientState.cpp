#include "stdafx.h"
#include "ClientState.h"
#include "Subscription.h"
#include "BrokerClient.h"

namespace me
{
ClientState::ClientState( )
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

std::queue<std::shared_ptr<ApplicationMessage>>& 
ClientState::GetPendingOutbound()
{
   return m_qPendingOutbound;
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

std::queue<std::shared_ptr<ApplicationMessage>>& 
ClientState::GetPendingPuback()
{
   return m_qPendingPuback;
}

void 
ClientState::AddPendingPuback( std::shared_ptr<ApplicationMessage> apMsg )
{
}

std::queue<std::shared_ptr<ApplicationMessage>>& 
ClientState::GetPendingPubrec()
{
   return m_qPendingPubrec;
}

void 
ClientState::AddPendingPubrec( std::shared_ptr<ApplicationMessage> apMsg )
{
}

std::queue<std::shared_ptr<ApplicationMessage>>& 
ClientState::GetPendingPubcomp()
{
   return m_qPendingPubcomp;
}

void 
ClientState::AddPendingPubcomp( std::shared_ptr<ApplicationMessage> apMsg )
{
}

void
ClientState::Subscribe( std::shared_ptr<Subscription> apSub )
{
   m_setSubscriptions.insert( apSub );
   apSub->RecordClient( shared_from_this() );
}

void
ClientState::Unsubscribe( std::shared_ptr<Subscription> apSub )
{
   m_setSubscriptions.erase( apSub );
   apSub->ReleaseClient( shared_from_this() );
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

}