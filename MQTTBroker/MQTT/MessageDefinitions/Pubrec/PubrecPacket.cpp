#include "stdafx.h"
#include "PubrecPacket.h"

namespace me
{
PubrecPacket::PubrecPacket( me::pcstring aszData, unsigned char aiFixedHeaderSize )
   : ControlPacketId( 5, 0x00 )
{
   // Parse the connect Packet
   const char* data = aszData->data();
   size_t i = aiFixedHeaderSize;

   // Variable Header Stuff
   const char* pVarHeader = data + i;
   // Packet Id
   setPacketId( (((unsigned short)pVarHeader[0]) << 8) | pVarHeader[1] );
   i += 2;
}

PubrecPacket::PubrecPacket( unsigned short aiPacketId )
   : ControlPacketId( aiPacketId, 5, 0x00 )
{
}


PubrecPacket::~PubrecPacket()
{
}

std::string
PubrecPacket::SerializeBody() const
{
   std::string szRetval;
   unsigned short id = GetPacketId();
   szRetval.append( 1, id >> 8 );
   szRetval.append( 1, id & 0x0F );
   return szRetval;
}
}