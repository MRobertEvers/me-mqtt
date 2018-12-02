#pragma once
#include "Definitions.h"
#include "Topic.h"
#include <string>
#include <map>

namespace me
{
class ApplicationMessage;
class RetainedTopic
{
public:
   RetainedTopic( Topic aTopic );
   ~RetainedTopic();

   void RecordMessage( std::shared_ptr<ApplicationMessage> apMsg );
   void ReleaseMessages(  );
   std::vector<std::shared_ptr<ApplicationMessage>> GetMessages();
   size_t GetMessageCount() const;

private:
   Topic m_Topic;
   std::vector<std::shared_ptr<ApplicationMessage>> m_vecMessage;
};

}