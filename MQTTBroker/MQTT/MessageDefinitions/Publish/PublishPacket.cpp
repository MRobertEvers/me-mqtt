#include "stdafx.h"
#include "PublishPacket.h"
#include "Utils.h"   
#include "MalformedPacket.h"

namespace me
{
PublishPacket::PublishPacket( me::pcstring aszData, unsigned char aiFixedHeaderSize )
   : ControlPacket( 3, 0x00 )
{
   m_iPacketId = 0;
   const char* data = aszData->data();

   // Fixed header reserved flags
   unsigned char iReserved = data[0] & 0xF;
   m_bDuplicateFlag = (iReserved & (1 << 3)) > 0;
   m_iQOS = ((iReserved & (0x3 << 1)) >> 1);
   if( m_iQOS == 3 )
   {
      throw MalformedPacket();
   }
   m_bRetainFlag = (iReserved & 1) > 0;

   // Variable Header stuff
   const char* pVarHeader = data + aiFixedHeaderSize;
   size_t i = aiFixedHeaderSize;
   // Topic Name
   size_t cur = utils::read_utf8_string_size( pVarHeader );
   m_szTopicName = std::make_shared<std::string>( pVarHeader + 2, cur );
   i += cur + 2;

   if( m_iQOS > 0 )
   {
      m_iPacketId = ((unsigned short)(data + i)[0]) << 8 | (data + i)[1];
      i += 2;
   }

   // Payload stuff
   const char* pPayload = data + i;
   m_szPayload = std::make_shared<std::string>( pPayload, aszData->size() - i );
}

PublishPacket::PublishPacket( 
   me::pcstring aszTopic, me::pcstring aszPayload, bool abDuplicate, 
   unsigned char aiQOS, bool abRetain, unsigned short aiPacketId )
   : ControlPacket(0x3, 0x00)
{
   // Set the reserved flags
   m_bDuplicateFlag = abDuplicate;
   m_iQOS = aiQOS;
   if( m_iQOS > 2 )
   {
      throw MalformedPacket();
   }
   m_bRetainFlag = abRetain;

   m_szTopicName = aszTopic;
   m_iPacketId = aiPacketId;
   m_szPayload = aszPayload;
}


PublishPacket::~PublishPacket()
{
}

bool
PublishPacket::GetDuplicateFlag() const
{
   return m_bDuplicateFlag;
}

unsigned char
PublishPacket::GetQOS() const
{
   return m_iQOS;
}

bool
PublishPacket::GetRetainFlag() const
{
   return m_bRetainFlag;
}

me::pcstring
PublishPacket::GetTopicName() const
{
   return m_szTopicName;
}

unsigned short
PublishPacket::GetPacketId() const
{
   return m_iPacketId;
}

me::pcstring
PublishPacket::GetPayload() const
{
   return m_szPayload;
}

unsigned char
PublishPacket::getFixedHeaderReserved() const
{
   unsigned char c = 0;
   if( m_bDuplicateFlag )
   {
      c |= 1 << 3;
   }
   c |= m_iQOS << 1;

   if( m_bRetainFlag )
   {
      c |= 1;
   }

   return c;
}

std::string
PublishPacket::SerializeBody() const
{
   size_t i = 0;
   std::string szRetval( m_szTopicName->size()+2, ' ' );

   // Encode topic name
   me::utils::encode_utf8_string( *m_szTopicName, &szRetval[0], m_szTopicName->size() + 2 );
   
   // Encode packet id
   if( m_iQOS > 0 )
   {
      szRetval.append( 1, m_iPacketId >> 8 );
      szRetval.append( 1, m_iPacketId & 0xFF );
   }

   // Encode payload
   szRetval += *m_szPayload;
   return szRetval;
}
}