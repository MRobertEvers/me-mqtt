#pragma once
#include "Definitions.h"
#include "ApplicationMessage.h"
#include <string>

namespace me
{

class TopicManager : public std::enable_shared_from_this<TopicManager>
{
public:
   TopicManager();
   ~TopicManager();

   void RetainMessage( me::pcstring apszTopicname, std::shared_ptr<ApplicationMessage> apMsg );
};

}