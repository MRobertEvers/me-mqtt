#include "stdafx.h"
#include "BrokerSession.h"
#include "BrokerClient.h"
#include "Broadcaster.h"
#include "BroadcasterClient.h"
#include "Asio/AsioConnection.h"

namespace me
{
BrokerSession::BrokerSession( 
   std::shared_ptr<Broadcaster> apBroadcaster,
   AsioConnection* apConnection )
   : m_pBroadcaster(apBroadcaster), m_pConnection(apConnection)
{

}


BrokerSession::~BrokerSession()
{
}

void 
BrokerSession::HandleConnect( std::shared_ptr<ConnectPacket> apPacket )
{
   if( m_pBrokerClient )
   {
      // TODO:
      throw;
   }
   else
   {
      // Look up if state exists. Etc.
      m_pBrokerClient = std::make_shared<BrokerClient>(
         apPacket, m_pBroadcaster->CreateClient(), m_pConnection );
      m_pBrokerClient->HandleConnect( apPacket );
   }
}

void
BrokerSession::HandleConnack( std::shared_ptr<ConnackPacket> apPacket )
{
   assertConnected();
   m_pBrokerClient->HandleConnack( apPacket );
}

void
BrokerSession::HandlePingReq( std::shared_ptr<PingReqPacket> apPacket )
{
   assertConnected();
   m_pBrokerClient->HandlePingReq( apPacket );
}

void 
BrokerSession::HandlePingResp( std::shared_ptr<PingRespPacket> apPacket )
{
   assertConnected();
   m_pBrokerClient->HandlePingResp( apPacket );
}

void
BrokerSession::HandleDisconnect( std::shared_ptr<DisconnectPacket> apPacket )
{
   assertConnected();
   m_pBrokerClient->HandleDisconnect( apPacket );
}

void 
BrokerSession::HandlePublish( std::shared_ptr<PublishPacket> apPacket )
{
   assertConnected();
   m_pBrokerClient->HandlePublish( apPacket );
}

void
BrokerSession::HandlePuback( std::shared_ptr<PubackPacket> apPacket )
{
   assertConnected();
   m_pBrokerClient->HandlePuback( apPacket );
}

void 
BrokerSession::HandlePubrec( std::shared_ptr<PubrecPacket> apPacket )
{
   assertConnected();
   m_pBrokerClient->HandlePubrec( apPacket );
}

void 
BrokerSession::HandlePubrel( std::shared_ptr<PubrelPacket> apPacket )
{
   assertConnected();
   m_pBrokerClient->HandlePubrel( apPacket );
}

void 
BrokerSession::HandlePubcomp( std::shared_ptr<PubcompPacket> apPacket )
{
   assertConnected();
   m_pBrokerClient->HandlePubcomp( apPacket );
}

void 
BrokerSession::HandleSubscribe( std::shared_ptr<SubscribePacket> apPacket )
{
   assertConnected();
   m_pBrokerClient->HandleSubscribe( apPacket );
}

void 
BrokerSession::HandleSuback( std::shared_ptr<SubackPacket> apPacket )
{
   assertConnected();
   m_pBrokerClient->HandleSuback( apPacket );
}

void
BrokerSession::HandleUnsubscribe( std::shared_ptr<UnsubscribePacket> apPacket )
{
   assertConnected();
   m_pBrokerClient->HandleUnsubscribe( apPacket );
}

void 
BrokerSession::HandleUnsuback( std::shared_ptr<UnsubackPacket> apPacket )
{
   assertConnected();
   m_pBrokerClient->HandleUnsuback( apPacket );
}

void 
BrokerSession::assertConnected()
{
   if( !m_pBrokerClient )
   {
      // TODO:
      throw;
   }
}
}