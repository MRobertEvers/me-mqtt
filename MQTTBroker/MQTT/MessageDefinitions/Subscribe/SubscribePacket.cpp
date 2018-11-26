#include "stdafx.h"
#include "SubscribePacket.h"
#include "Utils.h"
#include "MalformedPacket.h"
namespace me
{

SubscribePacket::SubscribePacket( me::pcstring aszData, unsigned char aiFixedHeaderSize )
   : ControlPacketId( 0, 8, 1 << 1 )
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
      if( i + cur + 2 + 1 > aszData->size() )
      {
         throw MalformedPacket();
      }

      unsigned char qos = (pPayload + cur + 2)[0];
      if( qos > 3 )
      {
         throw MalformedPacket();
      }

      m_vecTopicRequests.emplace_back( pPayload + 2, cur, qos );
      pPayload += cur + 2 + 1;
      i += cur + 2 + 1;
   }
}

SubscribePacket::SubscribePacket( unsigned short aiPacketId )
   : ControlPacketId( aiPacketId, 8, 1 << 1 )
{
}


SubscribePacket::~SubscribePacket()
{
}

std::vector<SubscribeRequest> const &
SubscribePacket::GetSubscribeRequests()
{
   return m_vecTopicRequests;
}

std::string
SubscribePacket::SerializeBody() const
{
   return std::string();
}
}