#pragma once

class ControlPacket;
class ConnectPacket;
class ConnackPacket;
class PingReqPacket;
class PingRespPacket;

class PacketHandler
{
public:
   virtual void HandlePacket( ControlPacket* rpPacket );

   virtual void HandleUnsupportedType( ControlPacket* rpPacket ) = 0;
   virtual void HandleConnect( ConnectPacket* rpPacket ) = 0;
   virtual void HandleConnack( ConnackPacket* rpPacket ) = 0;
   virtual void HandlePingReq( PingReqPacket* rpPacket ) = 0;
   virtual void HandlePingResp( PingRespPacket* rpPacket ) = 0;

private:
   void handle( ControlPacket* rpPacket ); // Error
   void handle( ConnectPacket* rpPacket );
   void handle( ConnackPacket* rpPacket );
   void handle( PingReqPacket* rpPacket );
   void handle( PingRespPacket* rpPacket );
};
