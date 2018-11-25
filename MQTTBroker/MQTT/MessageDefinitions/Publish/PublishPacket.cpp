#include "stdafx.h"
#include "PublishPacket.h"
#include "Utils.h"   
#include "MalformedPacket.h"

namespace me
{
PublishPacket::PublishPacket( std::string const& aszData, unsigned char aiFixedHeaderSize )
   : ControlPacket( 3, 0x00 )
{
   m_iPacketId = 0;
   const char* data = aszData.data();

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
   m_szTopicName = std::string( pVarHeader + 2, cur );
   i += cur + 2;

   if( m_iQOS > 0 )
   {
      m_iPacketId = ((unsigned short)(data + i)[0]) << 8 | (data + i)[1];
      i += 2;
   }

   // Payload stuff
   const char* pPayload = data + i;
   m_szPayload = std::string( pPayload, aszData.size() - i );
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

std::string
PublishPacket::GetTopicName() const
{
   return m_szTopicName;
}

unsigned short
PublishPacket::GetPacketId() const
{
   return m_iPacketId;
}

std::string
PublishPacket::GetPayload() const
{
   return m_szPayload;
}

unsigned char
PublishPacket::getFixedHeaderReserved() const
{
   return 0;
}

std::string
PublishPacket::SerializeBody() const
{
   return std::string();
}
}