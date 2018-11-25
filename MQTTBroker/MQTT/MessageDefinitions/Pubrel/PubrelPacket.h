#pragma once
#include "ControlPacketId.h"
namespace me
{
class PubrelPacket :
   public ControlPacketId
{
public:
   PubrelPacket( std::string const& aszData, unsigned char aiFixedHeaderSize );
   PubrelPacket( unsigned short aiPacketId );
   ~PubrelPacket();

   // Inherited via ControlPacket
   virtual std::string SerializeBody() const override;
};
}