#include "stdafx.h"
#include "SubackPacket.h"

namespace me
{
SubackPacket::SubackPacket( unsigned short aiPacketId, std::vector<unsigned char> avecResponses )
   : m_vecResponseCodes( avecResponses ), ControlPacketId( aiPacketId, 9, 0x00 )
{
   /*
   Allowed return codes:
0x00 - Success - Maximum QoS 0
0x01 - Success - Maximum QoS 1
0x02 - Success - Maximum QoS 2
0x80 - Failure
   */
}


SubackPacket::~SubackPacket()
{
}

std::string
SubackPacket::SerializeBody() const
{
   std::string szRetval;
   unsigned short id = GetPacketId();
   szRetval.append( 1, id >> 8 );
   szRetval.append( 1, id & 0xFF );

   for( auto resp : m_vecResponseCodes )
   {
      szRetval.append( 1, resp );
   }

   return szRetval;
}
}