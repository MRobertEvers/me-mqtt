#pragma once
#include "Definitions.h"
#include <string>

namespace me
{
class Subscription;

class SubscriptionStore
{
public:
   SubscriptionStore( );
   ~SubscriptionStore();

   // Filter needs to match exactly.
   std::shared_ptr<Subscription> GetSubscription( me::pcstring apszFilter );
   void RemoveSubscription( me::pcstring apszFilter );
};

}