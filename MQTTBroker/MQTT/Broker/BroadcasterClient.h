#pragma once
#include "Definitions.h"
#include <string>
#include <memory>

namespace me
{
class Broadcaster;
class BrokerClient;

// Acts as the synchronization bridge between the Broadcaster and the client.
class BroadcasterClient
{
public:
   // TODO: This should be a shared_ptr?
   BroadcasterClient( std::shared_ptr<Broadcaster> pBroadcaster );
   ~BroadcasterClient();

   void Connect( std::weak_ptr<BrokerClient> apClient );

   void 

   void BroadcastPublishMessage( 
      me::pcstring apszTopic, me::pcstring apszPayload, 
      unsigned char aiQOS, bool abRetain );

private:
   me::pcstring m_szClientName;
   std::shared_ptr<Broadcaster> m_pBroadcaster;
   std::weak_ptr<BrokerClient> m_pClient;
};
}