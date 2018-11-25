#pragma once
#include "ControlPacketId.h"

class PubcompPacket :
   public ControlPacketId
{
public:
   PubcompPacket( std::string const& aszData, unsigned char aiFixedHeaderSize );
   PubcompPacket(unsigned short aiPacketId);
   ~PubcompPacket();

   // Inherited via ControlPacket
   virtual std::string SerializeBody() const override;
};
