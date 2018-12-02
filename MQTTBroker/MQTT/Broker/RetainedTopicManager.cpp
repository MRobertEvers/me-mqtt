#include "stdafx.h"
#include "RetainedTopicManager.h"
#include "ClientState.h"
#include "Subscription.h"
#include "RetainedTopic.h"
#include "RetainedTopicStore.h"

namespace me
{
RetainedTopicManager::RetainedTopicManager()
{
   m_pTopicStore = std::make_shared<RetainedTopicStore>();
}

RetainedTopicManager::~RetainedTopicManager()
{

}

void 
RetainedTopicManager::RetainMessage( std::shared_ptr<ApplicationMessage> apMsg )
{
   auto topic = m_pTopicStore->Retain( apMsg );
   topic->RecordMessage( apMsg );
}

std::vector<std::shared_ptr<RetainedTopic>> 
RetainedTopicManager::GetRetainedMessages( me::pcstring apszTopicName )
{
   return m_pTopicStore->GetRetainedMessages(apszTopicName);
}

void 
RetainedTopicManager::ReleaseTopic( me::pcstring apszTopicFilter )
{
}

}
