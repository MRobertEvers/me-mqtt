#include "stdafx.h"
#include "PingReqPacket.h"


PingReqPacket::PingReqPacket()
   : ControlPacket(12, 0x00)
{
}


PingReqPacket::~PingReqPacket()
{
}

std::string
PingReqPacket::SerializeBody() const
{
   return std::string();
}
