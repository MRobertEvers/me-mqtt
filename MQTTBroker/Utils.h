#pragma once
#include "stdafx.h"
#include "Definitions.h"
#include <string>
namespace me
{
namespace utils
{

class pcview
{
public:
   pcview();
   pcview( char const* aszBase );
   pcview( me::pcstring apszSource, char const* aszStart, size_t aiLen );
   ~pcview();

   size_t size() const;
   char const* data() const;

   bool operator==( const pcview& rhs ) const;
   bool operator==( const std::string& rhs ) const;

private:
   me::pcstring m_szSource;
   char const* m_szStart;
   size_t m_iLen;
};

struct pcviewless
{
public:
   bool operator()( const me::utils::pcview& lhs, const me::utils::pcview& rhs ) const;
};

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