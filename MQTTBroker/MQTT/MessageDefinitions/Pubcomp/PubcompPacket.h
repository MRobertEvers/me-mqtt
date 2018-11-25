#pragma once
#include "ControlPacketId.h"
namespace me
{
class PubcompPacket :
   public ControlPacketId
{
public:
   PubcompPacket( me::pcstring aszData, unsigned char aiFixedHeaderSize );
   PubcompPacket( unsigned short aiPacketId );
   ~PubcompPacket();

   // Inherited via ControlPacket
   virtual std::string SerializeBody() const override;
};
}