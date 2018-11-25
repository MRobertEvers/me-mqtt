#include "stdafx.h"
#include "PubcompPacket.h"
namespace me
{

PubcompPacket::PubcompPacket( std::string const & aszData, unsigned char aiFixedHeaderSize )
   : ControlPacketId( 7, 0x00 )
{
   // Parse the connect Packet
   const char* data = aszData.data();
   size_t i = aiFixedHeaderSize;

   // Variable Header Stuff
   const char* pVarHeader = data + i;
   // Packet Id
   setPacketId( (((unsigned short)pVarHeader[0]) << 8) | pVarHeader[1] );
   i += 2;
}

PubcompPacket::PubcompPacket( unsigned short aiPacketId )
   : ControlPacketId( aiPacketId, 7, 0x00 )
{
}


PubcompPacket::~PubcompPacket()
{
}

std::string
PubcompPacket::SerializeBody() const
{
   std::string szRetval;
   unsigned short id = GetPacketId();
   szRetval.append( 1, id >> 8 );
   szRetval.append( 1, id & 0x0F );
   return szRetval;
}
}