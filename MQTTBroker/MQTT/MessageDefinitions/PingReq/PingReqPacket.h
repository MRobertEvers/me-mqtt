#pragma once
#include "ControlPacket.h"

class PingReqPacket :
   public ControlPacket
{
public:
   PingReqPacket();
   ~PingReqPacket();

   // Inherited via ControlPacket
   virtual std::string SerializeBody() const override;
};
