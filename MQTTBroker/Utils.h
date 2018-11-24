#pragma once
#include "stdafx.h"
#include <string>

namespace utils
{
   inline
   unsigned short read_utf8_string_size( const char* apData )
   {
      return ((unsigned short)apData[1]) << 8 & apData[0];
   }

   std::string
      read_utf8_string( const char* apData, size_t aDataSize );
}


