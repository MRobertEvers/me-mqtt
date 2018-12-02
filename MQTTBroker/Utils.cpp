#include "stdafx.h"
#include "Utils.h"


me::utils::pcview::pcview()
{
   m_szSource = (std::make_shared<std::string>());
   m_szStart = m_szSource->data();
   m_iLen = (0);
}

me::utils::pcview::pcview( char const * aszBase )
{
   m_szStart = (aszBase);
   m_iLen = strlen( aszBase );
}

me::utils::pcview::pcview( me::pcstring apszSource, char const* aszStart, size_t aiLen )
   : m_szSource( apszSource ), m_szStart( aszStart ), m_iLen( aiLen )
{

}

me::utils::pcview::~pcview()
{

}

size_t
me::utils::pcview::size() const
{
   return m_iLen;
}

char const*
me::utils::pcview::data() const
{
   return m_szStart;
}

bool
me::utils::pcview::operator==( const pcview& rhs ) const
{
   return memcmp( m_szStart, rhs.m_szStart, m_iLen );
}

bool
me::utils::pcview::operator==( const std::string& rhs ) const
{
   return memcmp( m_szStart, rhs.data(), m_iLen );
}

bool
me::utils::pcviewless::operator()( const me::utils::pcview& lhs, const me::utils::pcview& rhs ) const
{
   bool res = strncmp( lhs.data(), rhs.data(), lhs.size() ) < 0;
   return res;
}

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
