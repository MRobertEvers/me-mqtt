#include "stdafx.h"
#include "ControlPacket.h"
#include "MalformedPacket.h"


ControlPacket::ControlPacket( unsigned char aPacketType )
{
   // Constructor of derived class SHOULD set length
   setType( aPacketType );
}


ControlPacket::~ControlPacket()
{
}

ControlPacket::PacketTypes 
ControlPacket::GetType() const
{
   return m_iPacketType;
}

std::string
ControlPacket::Serialize() const
{
   std::string szRetval;
   szRetval.append( 1, m_iPacketType<<4 );

   std::string szBody = SerializeBody();
   size_t iSize = szBody.size();
   do
   {
      unsigned char byte = iSize % 0x80;
      iSize = iSize / 0x80;

      if( iSize > 0 )
      {
         byte = byte | 0x80;
      }
      szRetval.append( 1, byte );
   } while( iSize > 0 );
   
   szRetval.append( szBody );
   return szRetval;
}

void 
ControlPacket::setType( unsigned char aiType )
{
   if( aiType == 0 || aiType > 14 )
   {
      throw MalformedPacket();
   }

   m_iPacketType = (PacketTypes)aiType;
}
