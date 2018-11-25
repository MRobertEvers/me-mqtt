#pragma once
#include "ControlPacketId.h"
namespace me
{
class PubrecPacket :
   public ControlPacketId
{
public:
   PubrecPacket( me::pcstring aszData, unsigned char aiFixedHeaderSize );
   PubrecPacket( unsigned short aiPacketId );
   ~PubrecPacket();

   // Inherited via ControlPacket
   virtual std::string SerializeBody() const override;
};
}