#pragma once
#include "Definitions.h"
#include "ApplicationMessage.h"
#include "Broker\Subscription.h"
#include <string>
#include <queue>
#include <memory>
#include <set>

namespace me
{
class BrokerClient;
class Subscription;

class ClientState : public std::enable_shared_from_this<ClientState>
{
public:
   ClientState( );
   ~ClientState();

   void SetWatcher( std::weak_ptr<BrokerClient> apSource );
   void DisconnectWatch();

   std::queue<std::shared_ptr<ApplicationMessage>>& GetPendingOutbound();
   void AddPendingOutbound( std::shared_ptr<ApplicationMessage> apMsg );

   std::queue<std::shared_ptr<ApplicationMessage>>& GetPendingPuback();
   void AddPendingPuback( std::shared_ptr<ApplicationMessage> apMsg );

   std::queue<std::shared_ptr<ApplicationMessage>>& GetPendingPubrec();
   void AddPendingPubrec( std::shared_ptr<ApplicationMessage> apMsg );

   std::queue<std::shared_ptr<ApplicationMessage>>& GetPendingPubcomp();
   void AddPendingPubcomp( std::shared_ptr<ApplicationMessage> apMsg );

   void Subscribe( std::shared_ptr<Subscription> apSub );
   void Unsubscribe( std::shared_ptr<Subscription> apSub );
   void UnsubscribeAll();
private:
   // ClientStates own subscriptions. When a subscription no longer has any subscribers
   // then the subscription removes itself from the sub manager.
   std::set<std::shared_ptr<Subscription>> m_setSubscriptions;

   // Messages waiting to be sent out.
   std::queue<std::shared_ptr<ApplicationMessage>> m_qPendingOutbound;

   // Sent QOS 1 messages that have not been puback'd by the client.
   std::queue<std::shared_ptr<ApplicationMessage>> m_qPendingPuback;

   // Sent QOS 2 message that have not been Pubrec'd by the client.
   std::queue<std::shared_ptr<ApplicationMessage>> m_qPendingPubrec;
   // Sent QOS 2 message that have been sent Pubrel but have not been Pubcomp'd by the client.
   std::queue<std::shared_ptr<ApplicationMessage>> m_qPendingPubcomp;

   // STORE INFLIGHT IDS HERE??? TODO:

   std::weak_ptr<BrokerClient> m_pSource;
};
}