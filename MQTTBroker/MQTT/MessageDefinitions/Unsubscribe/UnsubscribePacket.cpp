#include "stdafx.h"
#include "UnsubscribePacket.h"
#include "MalformedPacket.h"
#include "Utils.h"
namespace me
{

UnsubscribePacket::UnsubscribePacket( me::pcstring aszData, unsigned char aiFixedHeaderSize )
   : ControlPacketId( 0, 10, 1 << 1 )
{
   if( ((aszData->data()[0] & 0xF) & (1 << 1)) == 0 )
   {
      throw MalformedPacket();
   }

   // Parse the connect Packet
   const char* data = aszData->data();
   size_t i = aiFixedHeaderSize;

   // Variable Header Stuff
   const char* pVarHeader = data + i;
   // Packet Id
   setPacketId( (((unsigned short)pVarHeader[0]) << 8) | pVarHeader[1] );
   i += 2;

   const char* pPayload = data + i;
   while( i < aszData->size() )
   {
      size_t cur = utils::read_utf8_string_size( pPayload );
      if( i + cur + 2 > aszData->size() )
      {
         throw MalformedPacket();
      }

      m_vecUnsubRequests.push_back( std::make_shared<std::string>( pPayload + 2, cur ));
      pPayload += cur + 2;
      i += cur + 2;
   }

}

UnsubscribePacket::UnsubscribePacket( unsigned short aiPacketId )
   : ControlPacketId( aiPacketId, 10, 1 << 1 )
{
}


UnsubscribePacket::~UnsubscribePacket()
{
}

std::vector<me::pcstring> 
UnsubscribePacket::GetUnsubscribeRequests() const
{
   return m_vecUnsubRequests;
}

std::string
UnsubscribePacket::SerializeBody() const
{
   return std::string();
}
}