#include "stdafx.h"
#include "PacketFactory.h"
#include "MalformedFixedHeader.h"
#include "ControlPacket.h"
#include "Connect\ConnectPacket.h"
#include "Connack\ConnackPacket.h"
#include "PingReq\PingReqPacket.h"


ControlPacket*
PacketFactory::GetPacket( std::string const& aszData, unsigned char aiFixedHeaderSize )
{
   char const* data = aszData.data();
   size_t size = aszData.size();
   if( size < 2 )
   {
      throw MalformedFixedHeader();
   }

   unsigned char iTypeAndFlags = data[0];
   unsigned char iType = (iTypeAndFlags & 0xF0) >> 4;
   if( iType == 0 || iType >= 0xF )
   {
      throw MalformedFixedHeader();
   }

   ControlPacket::PacketTypes type = (ControlPacket::PacketTypes)iType;
   switch( type )
   {
   case ControlPacket::PacketTypes::CONNECT:
      return new ConnectPacket( aszData, aiFixedHeaderSize );
      break;
   case ControlPacket::PacketTypes::PINGREQ:
      return new PingReqPacket();
      break;
   }
}
