#pragma once
#include "Definitions.h"
#include "SubscribeRequest.h"
#include <string>
#include <memory>

namespace me
{
class Broadcaster;
class ClientState;
class BrokerClient;
class ApplicationMessage;

// Acts as the synchronization bridge between the Broadcaster and the client.
class BroadcasterClient : public std::enable_shared_from_this<BroadcasterClient>
{
public:
   // TODO: This should be a shared_ptr?
   BroadcasterClient( std::shared_ptr<Broadcaster> pBroadcaster );
   ~BroadcasterClient();

   void ConnectClient( std::weak_ptr<BrokerClient> apClient );
   void DisconnectClient( bool abSaveState );
   std::weak_ptr<BrokerClient> GetClient() const;
   std::shared_ptr<ClientState> GetState();

   void BroadcastPublishMessage( 
      std::shared_ptr<ApplicationMessage> apMsg ) const;
   void SubscribeToTopics( unsigned short aiRequestIds, std::vector<SubscribeRequest> avecTopics ) const;
   void UnsubscribeFromTopics( unsigned short aiRequestIds, std::vector<me::pcstring> apszTopicFilter ) const;

private:
   std::shared_ptr<ClientState> m_pState;
   std::shared_ptr<Broadcaster> m_pBroadcaster;
   std::weak_ptr<BrokerClient> m_pClient;
};
}