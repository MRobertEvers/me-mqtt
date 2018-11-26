#include "stdafx.h"
#include "Topic.h"
#include "SubscriptionStore.h"
namespace me
{
SubscriptionStore::SubscriptionStore( )
{
}

SubscriptionStore::~SubscriptionStore()
{
}
std::shared_ptr<Subscription> SubscriptionStore::GetSubscription( me::pcstring apszFilter )
{
   return std::shared_ptr<Subscription>();
}
void SubscriptionStore::RemoveSubscription( me::pcstring apszFilter )
{
}
}
