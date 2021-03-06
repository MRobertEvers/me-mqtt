#include "stdafx.h"
#include "ConnectPacket.h"
#include "Utils.h"
#include "MalformedPacket.h"
namespace me
{

ConnectPacket::ConnectPacket(
   me::pcstring aszData, unsigned char aiFixedHeaderSize )
   : ControlPacket( aszData->data()[0] >> 4, 0x00 )
{
   // Parse the connect Packet
   const char* data = aszData->data();
   size_t size = aszData->size();
   size_t i = aiFixedHeaderSize;

   // Variable Header Stuff
   const char* pVarHeader = data + i;
   // Protocol Name
   size_t cur = utils::read_utf8_string_size( pVarHeader );
   m_szProtocolName = std::make_shared<const std::string>( pVarHeader + 2, cur );
   i += cur + 2;
   if( *m_szProtocolName != "MQTT" )
   {
      throw MalformedPacket();
   }

   // Protocol Level
   cur = 1;
   m_iProtocolLevel = pVarHeader[6];
   i += cur;
   if( m_iProtocolLevel != 4 )
   {
      throw MalformedPacket();
   }

   // ConnectFlags
   cur = 1;
   m_iFlags = pVarHeader[7];
   i += cur;
   if( (m_iFlags & 1) != 0 ) // Check reserved
   {
      throw MalformedPacket();
   }

   // Keep Alive timeout
   cur = 2;
   m_iKeepAlive = (((unsigned short)pVarHeader[8]) << 8) | pVarHeader[9];
   i += cur;

   // Payload
   const char* pPayload = data + i;
   // Client Identifier.
   cur = utils::read_utf8_string_size( pPayload );
   m_szClientName = std::make_shared<const std::string>( pPayload + 2, cur );
   i += cur + 2;
   pPayload += cur + 2;

   if( GetWillPresent() )
   {
      // Will Topic.
      cur = utils::read_utf8_string_size( pPayload );
      if( i + cur + 2 > size )
      {
         throw MalformedPacket();
      }

      m_szWillTopic = std::make_shared<const std::string>( pPayload + 2, cur );
      if( i + cur + 2 > size )
      {
         throw MalformedPacket();
      }

      i += cur + 2;
      pPayload += cur + 2;

      // Will Payload.
      cur = utils::read_utf8_string_size( pPayload );
      if( i + cur + 2 > size )
      {
         throw MalformedPacket();
      }

      m_szWillPayload = std::make_shared<const std::string>( pPayload + 2, cur );
      i += cur + 2;
      pPayload += cur + 2;
   }

   if( GetUsernamePresent() )
   {
      // Username.
      cur = utils::read_utf8_string_size( pPayload );
      if( i + cur + 2 > size )
      {
         throw MalformedPacket();
      }

      m_szUsername = std::make_shared<const std::string>( pPayload + 2, cur );
      i += cur + 2;
      pPayload += cur + 2;
   }

   if( GetPasswordPresent() )
   {
      // Password.
      cur = utils::read_utf8_string_size( pPayload );
      if( i + cur + 2 > size )
      {
         throw MalformedPacket();
      }

      m_szPassword = std::make_shared<const std::string>( pPayload + 2, cur );
      i += cur + 2;
      pPayload += cur + 2;
   }

   if( i != size )
   {
      throw MalformedPacket();
   }
}


ConnectPacket::~ConnectPacket()
{
}

me::pcstring
ConnectPacket::GetProtocolName() const
{
   return m_szProtocolName;
}

unsigned char
ConnectPacket::GetProtocolLevel() const
{
   return m_iProtocolLevel;
}

unsigned char
ConnectPacket::GetConnectFlags() const
{
   return m_iFlags;
}

unsigned short ConnectPacket::GetKeepAlive() const
{
   return m_iKeepAlive;
}

bool ConnectPacket::GetCleanSession() const
{
   return (GetConnectFlags() & (1 << 1)) > 0;
}

bool
ConnectPacket::GetWillPresent() const
{
   return (GetConnectFlags() & (1 << 2)) > 0;
}

unsigned char
ConnectPacket::GetWillQOS() const
{
   return (GetConnectFlags() & (0x3 << 3)) >> 3;
}

bool
ConnectPacket::GetWillRetain() const
{
   return (GetConnectFlags() & (1 << 5)) > 0;;
}

bool
ConnectPacket::GetUsernamePresent() const
{
   return (GetConnectFlags() & (1 << 6)) > 0;;
}

bool
ConnectPacket::GetPasswordPresent() const
{
   return (GetConnectFlags() & (1 << 7)) > 0;;
}

me::pcstring const
ConnectPacket::GetClientName() const
{
   return m_szClientName;
}

me::pcstring const
ConnectPacket::GetWillTopic() const
{
   return m_szWillTopic;
}

me::pcstring const
ConnectPacket::GetWillPayload() const
{
   return m_szWillPayload;
}

me::pcstring const
ConnectPacket::GetUsername() const
{
   return m_szUsername;
}

me::pcstring const
ConnectPacket::GetPassword() const
{
   return m_szPassword;
}

bool
ConnectPacket::SetCleanSession( bool abCleanSession )
{
   return false;
}

bool
ConnectPacket::SetWillPresent( bool abWillPresent )
{
   return false;
}

unsigned char
ConnectPacket::SetWillQOS( unsigned char abWillQOS )
{
   return 0;
}

bool
ConnectPacket::SetWillRetain( bool abWillRetain )
{
   return false;
}

bool
ConnectPacket::SetUsernamePresent( bool abUsernamePresent )
{
   return false;
}

bool
ConnectPacket::SetPasswordPresent( bool abPasswordPresent )
{
   return false;
}

std::string
ConnectPacket::SerializeBody() const
{
   return std::string();
}
}