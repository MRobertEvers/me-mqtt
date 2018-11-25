#pragma once
#include "IMessageHandler.h"
class AsioConnection;
namespace me
{
class BrokerClient;
class Broadcaster;

class BrokerSession : public IMessageHandler
{
public:
   BrokerSession( 
      std::shared_ptr<Broadcaster> apBroadcaster,
      AsioConnection* apConnection );
   ~BrokerSession();

   // Inherited via IMessageHandler
   virtual void HandleConnect( std::shared_ptr<ConnectPacket> apPacket ) override;
   virtual void HandleConnack( std::shared_ptr<ConnackPacket> apPacket ) override;
   virtual void HandlePingReq( std::shared_ptr<PingReqPacket> apPacket ) override;
   virtual void HandlePingResp( std::shared_ptr<PingRespPacket> apPacket ) override;
   virtual void HandleDisconnect( std::shared_ptr<DisconnectPacket> apPacket ) override;
   virtual void HandlePublish( std::shared_ptr<PublishPacket> apPacket ) override;
   virtual void HandlePuback( std::shared_ptr<PubackPacket> apPacket ) override;
   virtual void HandlePubrec( std::shared_ptr<PubrecPacket> apPacket ) override;
   virtual void HandlePubrel( std::shared_ptr<PubrelPacket> apPacket ) override;
   virtual void HandlePubcomp( std::shared_ptr<PubcompPacket> apPacket ) override;
   virtual void HandleSubscribe( std::shared_ptr<SubscribePacket> apPacket ) override;
   virtual void HandleSuback( std::shared_ptr<SubackPacket> apPacket ) override;
   virtual void HandleUnsubscribe( std::shared_ptr<UnsubscribePacket> apPacket ) override;
   virtual void HandleUnsuback( std::shared_ptr<UnsubackPacket> apPacket ) override;

private:
   void assertConnected();

   std::shared_ptr<BrokerClient> m_pBrokerClient;
   std::shared_ptr<Broadcaster> m_pBroadcaster;
   AsioConnection* m_pConnection;
};

}
