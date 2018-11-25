#pragma once
#include "ControlPacketId.h"

class PubrecPacket :
   public ControlPacketId
{
public:
   PubrecPacket( std::string const& aszData, unsigned char aiFixedHeaderSize );
   PubrecPacket(unsigned short aiPacketId);
   ~PubrecPacket();

   // Inherited via ControlPacket
   virtual std::string SerializeBody() const override;
};
