#include "stdafx.h"
#include "DisconnectPacket.h"


DisconnectPacket::DisconnectPacket()
   : ControlPacket(14, 0x00)
{
}


DisconnectPacket::~DisconnectPacket()
{
}

std::string
DisconnectPacket::SerializeBody() const
{
   return std::string();
}
