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
class ClientStateLedger;

// ClientState shared_ptrs are stored by the Subscription class and is owned by the ClientStateLedger.
class ClientState : public std::enable_shared_from_this<ClientState>
{
public:
   ClientState( 
      me::pcstring apszClientName,
      std::weak_ptr<ClientStateLedger> apManager );
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
   void NotifySubscribe( unsigned short aiRequestId, std::vector<unsigned char> avecSub );
   void Unsubscribe( std::shared_ptr<Subscription> apSub );
   void NotifyUnubscribe( unsigned short aiRequestId );
   void UnsubscribeAll();

   unsigned short GetNewPacketId() const;

   void Destroy();
private:

   // STATE HANDLED ON BROADCASTER STRAND

   // ClientStates own subscriptions. When a subscription no longer has any subscribers
   // then the subscription removes itself from the sub manager.
   std::set<std::shared_ptr<Subscription>> m_setSubscriptions;

   // Messages waiting to be sent out.
   std::queue<std::shared_ptr<ApplicationMessage>> m_qPendingOutbound;

   // STATE HANDLED BY BROKERCLIENT

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
   std::set<unsigned short> m_setInflightIds;

   // This is the "observer"
   std::weak_ptr<BrokerClient> m_pSource;

   std::weak_ptr<ClientStateLedger> m_pManager;
   me::pcstring m_pszClientName;
};
}