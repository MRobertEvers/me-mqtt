#include "stdafx.h"
#include "ConnackPacket.h"
#include "MalformedPacket.h"


ConnackPacket::ConnackPacket( bool abSessionPresent, unsigned char abCode )
   : m_bSessionPresent(abSessionPresent), ControlPacket( 0x02 )
{
   if( abCode > 5 )
   {
      throw MalformedPacket();
   }

   m_iReturnCode = ((ReturnCodes)abCode);
}


ConnackPacket::~ConnackPacket()
{
}

bool
ConnackPacket::GetSessionPresent() const
{
   return m_bSessionPresent;
}

ConnackPacket::ReturnCodes 
ConnackPacket::GetReturnCode() const
{
   return m_iReturnCode;
}

std::string 
ConnackPacket::SerializeBody() const
{
   std::string szRetval;
   unsigned char b1 = 0;
   if( m_bSessionPresent )
   {
      b1 = 1;
   }

   szRetval.append( 1, b1 );
   szRetval.append( 1, m_iReturnCode );
   return szRetval;
}
