#include "stdafx.h"
#include "PingRespPacket.h"


PingRespPacket::PingRespPacket()
   : ControlPacket(0x13)
{
}


PingRespPacket::~PingRespPacket()
{
}

std::string
PingRespPacket::SerializeBody() const
{
   return std::string();
}
