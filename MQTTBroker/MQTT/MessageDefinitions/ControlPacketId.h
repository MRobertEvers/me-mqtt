#pragma once
#include "ControlPacket.h"
namespace me
{
class ControlPacketId :
   public ControlPacket
{
public:
   ControlPacketId( unsigned char aPacketType, unsigned char aiReserved ); // For when we are loading from a stream.
   ControlPacketId( unsigned short aiPacketId, unsigned char aPacketType, unsigned char aiReserved );
   ~ControlPacketId();

   unsigned short GetPacketId() const;

protected:
   void setPacketId( unsigned short a );

private:
   unsigned short m_iPacketId;
};
}