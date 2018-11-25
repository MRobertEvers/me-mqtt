#pragma once
#include "Definitions.h"
#include "ApplicationMessage.h"
#include <string>

namespace me
{

class TopicManager
{
public:
   TopicManager();
   ~TopicManager();

   void RetainMessage( me::pcstring apszTopicname, std::shared_ptr<ApplicationMessage> apMsg );
};

}