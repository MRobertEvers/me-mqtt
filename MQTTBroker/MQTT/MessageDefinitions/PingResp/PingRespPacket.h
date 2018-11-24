#pragma once
#include "ControlPacket.h"

class PingRespPacket :
   public ControlPacket
{
public:
   PingRespPacket();
   ~PingRespPacket();

   // Inherited via ControlPacket
   virtual std::string SerializeBody() const override;
};

CONTROL_PACKET_HANDLER( PingResp )