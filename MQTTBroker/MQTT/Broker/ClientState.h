#pragma once
#include "Definitions.h"
#include "ApplicationMessage.h"
#include "Broker\Subscription.h"
#include <string>
#include <queue>
#include <memory>
#include <map>
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

   void AddPendingOutbound( std::shared_ptr<ApplicationMessage> apMsg );
   std::shared_ptr<ApplicationMessage> PeekNextOutbound();
   void ReleaseNextOutbound( );

   void AddPendingPuback( unsigned short id, std::shared_ptr<ApplicationMessage> apMsg );
   std::shared_ptr<ApplicationMessage> ReleasePendingPuback( unsigned short aiId );

   void AddPendingPubrec( unsigned short id, std::shared_ptr<ApplicationMessage> apMsg );
   std::shared_ptr<ApplicationMessage> ReleasePendingPubrec( unsigned short aiId );

   void AddPendingPubrel( unsigned short id, std::shared_ptr<ApplicationMessage> apMsg );
   std::shared_ptr<ApplicationMessage> ReleasePendingPubrel( unsigned short aiId );

   void AddPendingPubcomp( unsigned short id, std::shared_ptr<ApplicationMessage> apMsg );
   std::shared_ptr<ApplicationMessage> ReleasePendingPubcomp( unsigned short aiId );

   void Subscribe( std::shared_ptr<Subscription> apSub, unsigned char maxQOS );
   void Unsubscribe( std::shared_ptr<Subscription> apSub );
   void UnsubscribeAll();
private:
   // ClientStates own subscriptions. When a subscription no longer has any subscribers
   // then the subscription removes itself from the sub manager.
   std::set<std::shared_ptr<Subscription>> m_setSubscriptions;

   // Messages waiting to be sent out.
   std::queue<std::shared_ptr<ApplicationMessage>> m_qPendingOutbound;

   // 'Sent' below means TO THE CLIENT.
   // Sent QOS 1 messages that have not been puback'd by the client.
   std::map<unsigned short,std::shared_ptr<ApplicationMessage>> m_mapPendingPuback;

   // Sent QOS 2 message that have not been Pubrec'd by the client.
   std::map<unsigned short, std::shared_ptr<ApplicationMessage>> m_mapPendingPubrec;
   // Recved QOS 2 message that have been sent Pubrec have not been Pubrel'd by the client.
   std::map<unsigned short, std::shared_ptr<ApplicationMessage>> m_mapPendingPubrel;
   // Sent QOS 2 message that have been sent Pubrel but have not been Pubcomp'd by the client.
   std::map<unsigned short, std::shared_ptr<ApplicationMessage>> m_mapPendingPubcomp;

   // STORE INFLIGHT IDS HERE??? TODO:

   std::weak_ptr<BrokerClient> m_pSource;
};
}