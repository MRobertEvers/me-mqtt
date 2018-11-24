#include "stdafx.h"
#include "ControlPacket.h"


ControlPacket::ControlPacket( std::string aszData )
   : m_szData( std::forward<std::string>( aszData ))
{

}


ControlPacket::~ControlPacket()
{
}

ControlPacket::PacketTypes 
ControlPacket::GetType()
{
   return PacketTypes();
}

size_t ControlPacket::GetLength()
{
   return size_t();
}

char const*
ControlPacket::data() const noexcept
{
   return m_szData.data();
}

size_t 
ControlPacket::size() const noexcept
{
   return m_szData.size();
}
