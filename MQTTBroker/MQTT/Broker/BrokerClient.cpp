#include "stdafx.h"
#include "BrokerClient.h"
#include "AsioConnection.h"
#include "Connect\ConnectPacket.h"
#include "Connack\ConnackPacket.h"

BrokerClient::BrokerClient(AsioConnection* apConnection)
   : m_pConnection(apConnection)
{

}

BrokerClient::~BrokerClient()
{

}

void
BrokerClient::HandleUnsupportedType( ControlPacket * rpPacket )
{
}

void
BrokerClient::HandleConnect( ConnectPacket * rpPacket )
{
   if( !m_pConnectPacket )
   {
      // Take ownership!
      m_pConnectPacket = std::shared_ptr<ConnectPacket>( rpPacket );

      ConnackPacket response(false, 0x00);
      this->m_pConnection->WriteAsync( response.Serialize() );
   }
   else
   {
      // TODO:
      throw;
   }
}

void
BrokerClient::HandleConnack( ConnackPacket * rpPacket )
{
}

void 
BrokerClient::HandlePingReq( PingReqPacket * rpPacket )
{
}

void 
BrokerClient::HandlePingResp( PingRespPacket * rpPacket )
{
}
