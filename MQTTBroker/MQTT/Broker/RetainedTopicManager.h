#pragma once
#include "Definitions.h"
#include "ClientStateLedger.h"
#include "Broker\SubscribeRequest.h"
#include "TPathTree.h"
#include "Utils.h"
#include "Topic.h"
#include <string>
#include <map>


namespace me
{
class ApplicationMessage;
class RetainedTopic;
class ClientState;
class RetainedTopicStore;
class RetainedTopicManager : public std::enable_shared_from_this<RetainedTopicManager>
{
public:
   RetainedTopicManager();
   ~RetainedTopicManager();

   void RetainMessage( std::shared_ptr<ApplicationMessage> apMsg );
   std::vector<std::shared_ptr<RetainedTopic>> GetRetainedMessages(
      me::pcstring apszTopicName );

   void ReleaseTopic( me::pcstring apszTopicFilter );

private:
   std::map<me::pcstring, std::weak_ptr<RetainedTopic>, me::utils::pcstringless>
      m_mapFilters;

   std::shared_ptr<RetainedTopicStore> m_pTopicStore;
};

}