#include "stdafx.h"
#include "UnsubackPacket.h"

namespace me
{
UnsubackPacket::UnsubackPacket( unsigned short aiPacketId )
   : ControlPacketId( aiPacketId, 11, 0x00 )
{
}


UnsubackPacket::~UnsubackPacket()
{
}

std::string
UnsubackPacket::SerializeBody() const
{
   std::string szRetval;
   unsigned short id = GetPacketId();
   szRetval.append( 1, id >> 8 );
   szRetval.append( 1, id & 0x0F );
   return szRetval;
}
}