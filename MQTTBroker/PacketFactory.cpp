#include "stdafx.h"
#include "PacketFactory.h"
#include "MalformedFixedHeader.h"
#include "ControlPacket.h"


PacketFactory::PacketFactory()
{
}


PacketFactory::~PacketFactory()
{
}

std::shared_ptr<ControlPacket> 
PacketFactory::GetPacket( std::string aszData, size_t aiFixedHeaderSize )
{
   char const* data = aszData.data();
   size_t size = aszData.size();
   if( size < 2 )
   {
      throw MalformedFixedHeader();
   }

   unsigned char iTypeAndFlags = data[0];
   unsigned char iType = (iTypeAndFlags & 0xF) >> 4;
   if( iType == 0 || iType >= 0xF )
   {
      throw MalformedFixedHeader();
   }

   ControlPacket::PacketTypes type = (ControlPacket::PacketTypes)iType;
   switch( type )
   {
   case ControlPacket::PacketTypes::CONNECT:
      break;
      //return std::make_shared<ConnectPacket>(new ConnectPacket( std::forward<std::string>(aszData), aiFixedHeaderSize ));
   }
}
