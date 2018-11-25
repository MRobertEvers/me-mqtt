#pragma once
#include "Definitions.h"
#include <string>
#include <memory>

namespace me
{
class Broadcaster;
class BrokerClient;

// Acts as the synchronization bridge between the Broadcaster and the client.
class BroadcasterClient : public std::enable_shared_from_this<BroadcasterClient>
{
public:
   // TODO: This should be a shared_ptr?
   BroadcasterClient( std::shared_ptr<Broadcaster> pBroadcaster );
   ~BroadcasterClient();

   void ConnectClient( std::weak_ptr<BrokerClient> apClient );
   std::weak_ptr<BrokerClient> GetClient() const;
   me::pcstring GetClientName() const;

   void BroadcastPublishMessage( 
      me::pcstring apszTopic, me::pcstring apszPayload, 
      unsigned char aiQOS, bool abRetain ) const;
   void SubscribeToTopic( me::pcstring apszTopicFilter ) const;

private:
   me::pcstring m_szClientName;
   std::shared_ptr<Broadcaster> m_pBroadcaster;
   std::weak_ptr<BrokerClient> m_pClient;
};
}