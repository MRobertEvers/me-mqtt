#pragma once
#include "ControlPacket.h"
namespace me
{
class PingReqPacket :
   public ControlPacket
{
public:
   PingReqPacket();
   ~PingReqPacket();

   // Inherited via ControlPacket
   virtual std::string SerializeBody() const override;
};
}