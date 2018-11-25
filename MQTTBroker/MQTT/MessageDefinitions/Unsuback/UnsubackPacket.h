#pragma once
#include "ControlPacketId.h"
namespace me
{
class UnsubackPacket :
   public ControlPacketId
{
public:
   UnsubackPacket( unsigned short aiPacketId );
   ~UnsubackPacket();

   // Inherited via ControlPacket
   virtual std::string SerializeBody() const override;
};
}