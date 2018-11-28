#pragma once
#include "AsioService.h"
#include "ApplicationMessage.h"
#include "Definitions.h"
#include "SubscribeRequest.h"
#include <map>

namespace me
{
class BroadcasterClient;
class ClientState;
class ClientStateLedger;
class SubscriptionManager;
class TopicManager;

class Broadcaster : public std::enable_shared_from_this<Broadcaster>
{
public:
   Broadcaster( std::shared_ptr<AsioService> apIOService );
   ~Broadcaster();

   void Subscribe( std::shared_ptr<ClientState> apClient, 
      unsigned short aiRequestId, std::vector<SubscribeRequest> avecTopics );
   void Unsubscribe( std::shared_ptr<ClientState> apClient,
      unsigned short aiRequestId, std::vector<me::pcstring> avecTopics );
   void BroadcastMessage( std::shared_ptr<ApplicationMessage> apMessage );
   std::shared_ptr<BroadcasterClient> CreateClient();

   std::shared_ptr<ClientState> ConnectClient( std::weak_ptr<BroadcasterClient> apClient );
   void DisconnectClient( std::weak_ptr<BroadcasterClient> apClient );

private:
   void subscribe( std::shared_ptr<ClientState> apClient,
      unsigned short aiRequestId, std::vector<SubscribeRequest> avecTopics );
   void unsubscribe( std::shared_ptr<ClientState> apClient,
      unsigned short aiRequestId, std::vector<me::pcstring> avecTopics );
   void broadcast( std::shared_ptr<ApplicationMessage> apMessage );
   void disconnect( std::weak_ptr<BroadcasterClient> apClient );

   std::shared_ptr<AsioService> m_pService;
   std::shared_ptr<asio::io_service::work> m_pWork;
   std::shared_ptr<asio::io_context::strand> m_pStrand;

   std::shared_ptr<SubscriptionManager> m_pSubscriptionManager;
   std::shared_ptr<TopicManager> m_pTopicManager;
   std::shared_ptr<ClientStateLedger> m_pClients;
};
}