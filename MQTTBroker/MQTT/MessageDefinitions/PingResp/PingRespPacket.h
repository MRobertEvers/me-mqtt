#pragma once
#include "ControlPacket.h"
namespace me
{
class PingRespPacket :
   public ControlPacket
{
public:
   PingRespPacket();
   ~PingRespPacket();

   // Inherited via ControlPacket
   virtual std::string SerializeBody() const override;
};
}