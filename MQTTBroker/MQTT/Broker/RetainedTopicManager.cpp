#include "stdafx.h"
#include "RetainedTopicManager.h"
#include "ClientState.h"
#include "Subscription.h"
#include "RetainedTopic.h"
#include "RetainedTopicStore.h"
#include "ApplicationMessage.h"

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
   if( apMsg->GetPayload()->size() != 0 )
   {
      auto topic = m_pTopicStore->Retain( apMsg );
      topic->RecordMessage( apMsg );
   }
   else
   {
      m_pTopicStore->ReleaseRetainedMessage( apMsg->GetTopic() );
   }
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
