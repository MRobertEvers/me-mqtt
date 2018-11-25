#pragma once
#include "Definitions.h"

namespace me
{

class ClientStateLedger
{
public:
   ClientStateLedger();
   ~ClientStateLedger();

   void FindClient( me::pcstring apszClientname );
   void CreateClient( me::pcstring apszClientname );
   void DeleteClient( me::pcstring apszClientname );


};

}