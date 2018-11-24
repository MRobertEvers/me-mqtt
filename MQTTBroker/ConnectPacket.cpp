#include "stdafx.h"
#include "ConnectPacket.h"
#include "Utils.h"


ConnectPacket::ConnectPacket( std::string& aszData, unsigned char aiFixedHeaderSize )
   : ControlPacket( std::forward<std::string>(aszData) )
{
   /*m_iVariableHeaderStart = aiFixedHeaderSize;
   m_iPayloadStart = aiFixedHeaderSize + 4+GetProtocolName().size();

   m_iClientNameStart = m_iPayloadStart;
   size_t last = m_iClientNameStart;
   if( GetWillPresent() )
   {
      m_iWillTopicStart = last + 2 + utils::read_utf8_string_size( data() + last );
      last = m_iWillTopicStart;
      m_iWillPayloadStart = last + 2 + utils::read_utf8_string_size( data() + last );
      last = m_iWillPayloadStart;
   }
   if( GetUsernamePresent() )
   {
      m_iUsernameStart = last + 2 + utils::read_utf8_string_size( data() + last );
      last = m_iUsernameStart;
   }
   if( GetPasswordPresent() )
   {
      m_iPasswordStart = last + 2 + utils::read_utf8_string_size( data() + last );
   }*/
}


ConnectPacket::~ConnectPacket()
{
}
//
//std::string 
//ConnectPacket::GetProtocolName() const
//{
//   return utils::read_utf8_string(GetBody(), GetLength());
//}
//
//unsigned char ConnectPacket::GetProtocolLevel() const
//{
//   return GetVariableHeader()[6];
//}
//
//unsigned char ConnectPacket::GetConnectFlags() const
//{
//   return GetVariableHeader()[7];
//}
//
//unsigned short ConnectPacket::GetKeepAlive() const
//{
//   return GetVariableHeader()[8] << 8 & GetVariableHeader()[9];
//}
//
//bool ConnectPacket::GetCleanSession() const
//{
//   return (GetConnectFlags() & (1<<1)) > 0;
//}
//
//bool ConnectPacket::GetWillPresent() const
//{
//   return (GetConnectFlags() & (1 << 2)) > 0;
//}
//
//unsigned char ConnectPacket::GetWillQOS() const
//{
//   return (GetConnectFlags() & (0x3 << 3)) >> 3;
//}
//
//bool ConnectPacket::GetWillRetain() const
//{
//   return (GetConnectFlags() & (1 << 5)) > 0;;
//}
//
//bool ConnectPacket::GetUsernamePresent() const
//{
//   return (GetConnectFlags() & (1 << 6)) > 0;;
//}
//
//bool ConnectPacket::GetPasswordPresent() const
//{
//   return (GetConnectFlags() & (1 << 7)) > 0;;
//}
//
//std::string
//ConnectPacket::GetClientName() const
//{
//   return std::string(data()+m_iClientNameStart, m_iWillTopicStart - m_iClientNameStart);
//}
//
//std::string 
//ConnectPacket::GetWillTopic() const
//{
//   return std::string( data() + m_iWillTopicStart, m_iWillPayloadStart - m_iWillTopicStart );
//}
//
//std::string
//ConnectPacket::GetWillPayload() const
//{
//   return std::string( data() + m_iWillPayloadStart, m_iUsernameStart - m_iWillPayloadStart );
//}
//
//std::string 
//ConnectPacket::GetUsername() const
//{
//   return std::string( data() + m_iUsernameStart, m_iPasswordStart - m_iUsernameStart );
//}
//
//std::string
//ConnectPacket::GetPassword() const
//{
//   return std::string( data() + m_iPasswordStart, size() - m_iPasswordStart );
//}
//
//char const * 
//ConnectPacket::GetVariableHeader() const
//{
//   return data()+m_iVariableHeaderStart;
//}
//
//char const * ConnectPacket::GetPayload() const
//{
//   return data()+m_iPayloadStart;
//}
