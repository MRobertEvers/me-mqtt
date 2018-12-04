#pragma once
#include "IMessageHandler.h"
#include "asio.hpp"
#include <memory>

class AsioConnection;

namespace me
{
class ApplicationMessage;
class BroadcasterClient;

class BrokerClient : public IMessageHandler, public std::enable_shared_from_this<BrokerClient>
{
public:
   BrokerClient(
      std::shared_ptr<ConnectPacket> apConnectPacket,
      std::shared_ptr<BroadcasterClient> apBroadcaster,
      AsioConnection* apConnection );
   virtual ~BrokerClient();

   void Accept( bool abSessionPresent );
   void Reject( unsigned char aiReason );
   void Respond( bool abSessionPresent, unsigned char aiResponse );
   void Disconnect();
   void Cleanup();

   me::pcstring GetClientName() const;

   void PublishTo( std::shared_ptr<ApplicationMessage> apMsg );
   void NotifyUnsubscribed( unsigned short aiRequestId );
   void NotifySubscribed( unsigned short aiRequestId, std::vector<unsigned char> avecResponses);

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
   void publishTo( std::shared_ptr<ApplicationMessage> apMsg );
   void notifyUnsubscribed( unsigned short aiPacketId );
   void notifySubscribed( unsigned short aiPacketId, std::vector<unsigned char> avecResponses );
   void resetTimeout();
   std::shared_ptr<asio::steady_timer> m_pConnectTimer;
   std::shared_ptr<ConnectPacket> m_pConnectPacket;
   std::shared_ptr<BroadcasterClient> m_pBroadcaster;
   AsioConnection* m_pConnection;

   std::shared_ptr<ApplicationMessage> m_pWillMessage;
};

}