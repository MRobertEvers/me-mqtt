#include "stdafx.h"
#include "PacketHandler.h"

void 
PacketHandler::HandlePacket( ControlPacket * rpPacket )
{
   handle( rpPacket );
}

void 
PacketHandler::handle( ControlPacket * rpPacket )
{
   HandleUnsupportedType( rpPacket );
}

void
PacketHandler::handle( ConnectPacket * rpPacket )
{
   HandleConnect( rpPacket );
}

void
PacketHandler::handle( ConnackPacket * rpPacket )
{
   HandleConnack( rpPacket );
}

void
PacketHandler::handle( PingReqPacket * rpPacket )
{
   HandlePingReq( rpPacket );
}

void
PacketHandler::handle( PingRespPacket * rpPacket )
{
   HandlePingResp( rpPacket );
}
