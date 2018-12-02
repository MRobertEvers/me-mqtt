#include "stdafx.h"
#include "RetainedTopic.h"
#include "ClientState.h"
#include "SubscriptionManager.h"
#include "ApplicationMessage.h"
#include "Topic.h"
#include <algorithm>

namespace me
{
RetainedTopic::RetainedTopic( Topic aTopic )
   : m_Topic(aTopic)
{
}

RetainedTopic::~RetainedTopic()
{
}

void RetainedTopic::RecordMessage( std::shared_ptr<ApplicationMessage> apMsg )
{
   m_vecMessage.push_back( apMsg );
}

void RetainedTopic::ReleaseMessages()
{
   m_vecMessage.clear();
}

std::vector<std::shared_ptr<ApplicationMessage>>
RetainedTopic::GetMessages()
{

   return m_vecMessage;
}

size_t 
RetainedTopic::GetMessageCount() const
{
   return m_vecMessage.size();
}


}
