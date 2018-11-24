#pragma once
#include "PacketHandler.h"
#include <memory>

class ControlPacket;
class ConnectPacket;
class AsioConnection;

class BrokerClient: public PacketHandler
{
public:
   BrokerClient( AsioConnection* apConnection );
   virtual ~BrokerClient();

   // Inherited via PacketHandler
   virtual void HandleUnsupportedType( ControlPacket * rpPacket ) override;
   virtual void HandleConnect( ConnectPacket * rpPacket ) override;
   virtual void HandleConnack( ConnackPacket * rpPacket ) override;
   virtual void HandlePingReq( PingReqPacket * rpPacket ) override;
   virtual void HandlePingResp( PingRespPacket * rpPacket ) override;

private:
   std::shared_ptr<ConnectPacket> m_pConnectPacket;
   AsioConnection* m_pConnection;
};