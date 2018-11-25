#include "stdafx.h"
#include "ClientState.h"
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

}