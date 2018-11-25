#include "stdafx.h"
#include "ControlPacketId.h"

ControlPacketId::ControlPacketId( unsigned char aPacketType, unsigned char aiReserved )
   : m_iPacketId( 0 ), ControlPacket( aPacketType, aiReserved )
{
}

ControlPacketId::ControlPacketId( unsigned short aiPacketId, unsigned char aPacketType, unsigned char aiReserved )
   : m_iPacketId(aiPacketId), ControlPacket( aPacketType, aiReserved )
{
}


ControlPacketId::~ControlPacketId()
{
}

unsigned short 
ControlPacketId::GetPacketId() const
{
   return m_iPacketId;
}

void 
ControlPacketId::setPacketId( unsigned short aiPacketId)
{
   m_iPacketId = aiPacketId;
}

