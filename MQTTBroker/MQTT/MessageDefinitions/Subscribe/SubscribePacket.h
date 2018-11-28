#pragma once
#include "ControlPacketId.h"
#include "Definitions.h"
#include "Broker\SubscribeRequest.h"


namespace me
{

class SubscribePacket :
   public ControlPacketId
{
public:
   SubscribePacket( me::pcstring aszData, unsigned char aiFixedHeaderSize );
   SubscribePacket( unsigned short aiPacketId );
   ~SubscribePacket();

   std::vector<SubscribeRequest> const& GetSubscribeRequests();

   // Inherited via ControlPacket
   virtual std::string SerializeBody() const override;

private:
   std::vector<SubscribeRequest> m_vecTopicRequests;
};
}