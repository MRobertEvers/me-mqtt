#pragma once
#include "Definitions.h"
#include "Utils.h"
#include "Topic.h"
#include "TPathTree.h"
#include "ApplicationMessage.h"
#include <string>
#include <memory>

namespace me
{
class RetainedTopic;
class RetainedTopicStore;
typedef Node<
   std::shared_ptr<RetainedTopic>,
   Topic,
   RetainedTopicStore*
> RetainTreeNode;

class RetainedTopicStore
{
public:
   RetainedTopicStore(  );
   ~RetainedTopicStore();

   std::shared_ptr<RetainedTopic> Retain( std::shared_ptr<ApplicationMessage> apMsg );
   std::vector<std::shared_ptr<RetainedTopic>> GetRetainedMessages( Topic apszFilter );
   void ReleaseRetainedMessage( Topic apszFilter );

   std::shared_ptr<RetainedTopic> Create( Topic aTopic );

private:
   std::shared_ptr<RetainTreeNode> m_pNewRoot;

   std::map<me::pcstring, std::shared_ptr<RetainedTopic>, me::utils::pcstringless> m_mapFastSubLookup;
};

}