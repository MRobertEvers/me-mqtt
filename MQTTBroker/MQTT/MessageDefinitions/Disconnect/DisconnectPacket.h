#pragma once
#include "ControlPacket.h"
namespace me
{
class DisconnectPacket :
   public ControlPacket
{
public:
   DisconnectPacket();
   ~DisconnectPacket();

   // Inherited via ControlPacket
   virtual std::string SerializeBody() const override;
};
}