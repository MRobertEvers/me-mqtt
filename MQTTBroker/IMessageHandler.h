#pragma once

#include "Connect/ConnectPacket.h"
#include "Connack/ConnackPacket.h"
#include "PingReq/PingReqPacket.h"
#include "PingResp/PingRespPacket.h"
#include "Disconnect/DisconnectPacket.h"
#include "Publish/PublishPacket.h"
#include "Puback/PubackPacket.h"
#include "Pubrec/PubrecPacket.h"
#include "Pubrel/PubrelPacket.h"
#include "Pubcomp/PubcompPacket.h"
#include "Subscribe/SubscribePacket.h"
#include "Suback/SubackPacket.h"
#include "Unsubscribe/UnsubscribePacket.h"
#include "Unsuback/UnsubackPacket.h"

#include <memory>

class IMessageHandler
{
public:
   virtual void HandleConnect( std::shared_ptr<ConnectPacket> apPacket ) = 0;
   virtual void HandleConnack( std::shared_ptr<ConnackPacket> apPacket ) = 0;
   virtual void HandlePingReq( std::shared_ptr<PingReqPacket> apPacket ) = 0;
   virtual void HandlePingResp( std::shared_ptr<PingRespPacket> apPacket ) = 0;
   virtual void HandleDisconnect( std::shared_ptr<DisconnectPacket> apPacket ) = 0;
   virtual void HandlePublish( std::shared_ptr<PublishPacket> apPacket ) = 0;
   virtual void HandlePuback( std::shared_ptr<PubackPacket> apPacket ) = 0;
   virtual void HandlePubrec( std::shared_ptr<PubrecPacket> apPacket ) = 0;
   virtual void HandlePubrel( std::shared_ptr<PubrelPacket> apPacket ) = 0;
   virtual void HandlePubcomp( std::shared_ptr<PubcompPacket> apPacket ) = 0;
   virtual void HandleSubscribe( std::shared_ptr<SubscribePacket> apPacket ) = 0;
   virtual void HandleSuback( std::shared_ptr<SubackPacket> apPacket ) = 0;

   virtual void HandleUnsubscribe( std::shared_ptr<UnsubscribePacket> apPacket ) = 0;
   virtual void HandleUnsuback( std::shared_ptr<UnsubackPacket> apPacket ) = 0;
};

