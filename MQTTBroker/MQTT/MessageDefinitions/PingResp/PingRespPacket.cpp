#include "stdafx.h"
#include "PingRespPacket.h"


PingRespPacket::PingRespPacket()
   : ControlPacket(13, 0x00)
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
