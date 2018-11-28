#pragma once
#include "stdafx.h"
#include "Definitions.h"
#include <string>
namespace me
{
namespace utils
{
struct pcstringless
{
public:
   bool operator()( const me::pcstring& lhs, const me::pcstring& rhs ) const;
};

unsigned short read_utf8_string_size( const char* apData );

std::string read_utf8_string( const char* apData, size_t aDataSize );

void encode_utf8_string( 
   std::string const& aszStr, char* rpBuf, unsigned short aiBufSize );
}
}