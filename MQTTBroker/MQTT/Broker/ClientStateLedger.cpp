#include "stdafx.h"
#include "ClientStateLedger.h"

namespace me
{

ClientStateLedger::ClientStateLedger()
{
}


ClientStateLedger::~ClientStateLedger()
{
}

std::shared_ptr<ClientState>
ClientStateLedger::FindClient( me::pcstring apszClientname )
{
   auto pClient = m_mapStates.find( apszClientname );
   if( pClient != m_mapStates.end() )
   {
      return pClient->second;
   }
   else
   {
      return nullptr;
   }
}

std::shared_ptr<ClientState>
ClientStateLedger::CreateClient( me::pcstring apszClientname )
{
   if( FindClient( apszClientname ) != nullptr )
   {
      // TODO:
      throw;
   }

   std::shared_ptr<ClientState> p;
}

void 
ClientStateLedger::DeleteClient( me::pcstring apszClientname )
{
}
}
