#include "stdafx.h"
#include "PubrelPacket.h"
#include "MalformedPacket.h"


PubrelPacket::PubrelPacket( std::string const & aszData, unsigned char aiFixedHeaderSize )
   : ControlPacketId( 5, 1 << 1 )
{
   if( ((aszData[0] & 0xF) & (1 << 1)) == 0 )
   {
      throw MalformedPacket();
   }

   // Parse the connect Packet
   const char* data = aszData.data();
   size_t i = aiFixedHeaderSize;

   // Variable Header Stuff
   const char* pVarHeader = data + i;
   // Packet Id
   setPacketId( (((unsigned short)pVarHeader[0]) << 8) | pVarHeader[1] );
   i += 2;
}

PubrelPacket::PubrelPacket(unsigned short aiPacketId)
  : ControlPacketId(aiPacketId, 6, 1 << 1)
{
}


PubrelPacket::~PubrelPacket()
{
}

std::string
PubrelPacket::SerializeBody() const
{
   std::string szRetval;
   unsigned short id = GetPacketId();
   szRetval.append( 1, id >> 8 );
   szRetval.append( 1, id & 0x0F );
   return szRetval;
}
