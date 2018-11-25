#pragma once
#include "ControlPacketId.h"
namespace me
{
class PubackPacket :
   public ControlPacketId
{
public:
   PubackPacket( std::string const& aszData, unsigned char aiFixedHeaderSize );
   PubackPacket( unsigned short aiPacketId );
   ~PubackPacket();

   // Inherited via ControlPacket
   virtual std::string SerializeBody() const override;
};
}