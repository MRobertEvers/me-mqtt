#include "stdafx.h"
#include "PingReqPacket.h"


PingReqPacket::PingReqPacket()
   : ControlPacket(0x12)
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
