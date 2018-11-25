#pragma once
#include "Definitions.h"
#include <map>

namespace me
{
class ClientState;

class ClientStateLedger
{
public:
   ClientStateLedger();
   ~ClientStateLedger();

   std::shared_ptr<ClientState> FindClient( me::pcstring apszClientname );
   std::shared_ptr<ClientState> CreateClient( me::pcstring apszClientname );
   void DeleteClient( me::pcstring apszClientname );

private:
   std::map<me::pcstring, std::shared_ptr<ClientState>> m_mapStates;
};

}