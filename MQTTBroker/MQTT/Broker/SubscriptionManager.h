#pragma once
#include "Definitions.h"
#include "ClientStateLedger.h"
#include <string>

namespace me
{
class SubscriptionManager
{
public:
   SubscriptionManager(  );
   ~SubscriptionManager();

   void Subscribe( me::pcstring apszClientName, me::pcstring apszTopicFilter );
};

}