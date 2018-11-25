#include "stdafx.h"
#include "Utils.h"

namespace me
{
std::string
utils::read_utf8_string( const char* apData, size_t aDataSize )
{
   std::string szRetval;
   unsigned short len = read_utf8_string_size( apData );
   if( len <= aDataSize )
   {
      szRetval.append( apData + 2, len );// std::string( apData + 2, len );
   }
   return szRetval;
}
}