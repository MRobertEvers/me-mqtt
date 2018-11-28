#include "stdafx.h"
#include "Utils.h"

bool 
me::utils::pcstringless::operator()( const me::pcstring& lhs, const me::pcstring& rhs ) const
{
   return *lhs < *rhs;
}

unsigned short
me::utils::read_utf8_string_size( const char* apData )
{
   return (((unsigned short)apData[0]) << 8) | apData[1];
}

std::string
me::utils::read_utf8_string( const char* apData, size_t aDataSize )
{
   std::string szRetval;
   unsigned short len = read_utf8_string_size( apData );
   if( len <= aDataSize )
   {
      szRetval.append( apData + 2, len );// std::string( apData + 2, len );
   }
   return szRetval;
}

void
me::utils::encode_utf8_string( std::string const& aszStr, char* rpBuf, unsigned short aiBufSize )
{
   char const* data = aszStr.data();
   size_t len = aszStr.size() & 0xFFFF;

   rpBuf[0] = len >> 8;
   rpBuf[1] = len & 0x00FF;
   memcpy_s( rpBuf+2, aiBufSize-2, data, len );
}
