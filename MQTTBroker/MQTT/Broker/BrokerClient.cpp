#include "stdafx.h"
#include "BrokerClient.h"
#include "AsioConnection.h"
#include "Connect\ConnectPacket.h"
#include "Connack\ConnackPacket.h"
#include "Broker/BroadcasterClient.h"
#include "PingResp\PingRespPacket.h"

namespace me
{

BrokerClient::BrokerClient(
   std::shared_ptr<BroadcasterClient> apBroadcaster,
   AsioConnection* apConnection )
   : m_pConnection( apConnection ), m_pBroadcaster(apBroadcaster)
{

}

BrokerClient::~BrokerClient()
{

}

me::pcstring
BrokerClient::GetClientName() const
{
   return m_pConnectPacket->GetClientName();
}

void
BrokerClient::HandleConnect( std::shared_ptr<ConnectPacket> apPacket )
{
   if( !m_pConnectPacket )
   {
      m_pConnectPacket = apPacket;

      m_pBroadcaster->SetClientName( apPacket->GetClientName() );

      m_pConnection->WriteAsync( ConnackPacket( 0, 0x00 ).Serialize() );
   }
   else
   {
      // TODO:
      throw;
   }
}

void
BrokerClient::HandleConnack( std::shared_ptr<ConnackPacket> apPacket )
{
   assertConnected();
}

void
BrokerClient::HandlePingReq( std::shared_ptr<PingReqPacket> apPacket )
{
   assertConnected();

   m_pConnection->WriteAsync( PingRespPacket().Serialize() );
}

void
BrokerClient::HandlePingResp( std::shared_ptr<PingRespPacket> apPacket )
{
   assertConnected();
}

void
BrokerClient::HandleDisconnect( std::shared_ptr<DisconnectPacket> apPacket )
{
   assertConnected();

   m_pConnection->Stop();
}

void
BrokerClient::HandlePublish( std::shared_ptr<PublishPacket> apPacket )
{
   assertConnected();

   m_pBroadcaster->BroadcastPublishMessage(
      apPacket->GetTopicName(), apPacket->GetPayload(),
      apPacket->GetQOS(), apPacket->GetRetainFlag()
   );

   unsigned char iQos = apPacket->GetQOS();
   if( iQos == 1 )
   {
      m_pConnection->WriteAsync( PubackPacket( apPacket->GetPacketId() ).Serialize() );
   }
   else if( iQos == 2 )
   {
      m_pConnection->WriteAsync( PubrecPacket( apPacket->GetPacketId() ).Serialize() );
   }
}

void
BrokerClient::HandlePuback( std::shared_ptr<PubackPacket> apPacket )
{
   assertConnected();
}

void
BrokerClient::HandlePubrec( std::shared_ptr<PubrecPacket> apPacket )
{
   assertConnected();

   m_pConnection->WriteAsync( PubrelPacket( apPacket->GetPacketId() ).Serialize() );
}

void
BrokerClient::HandlePubrel( std::shared_ptr<PubrelPacket> apPacket )
{
   assertConnected();

   m_pConnection->WriteAsync( PubcompPacket( apPacket->GetPacketId() ).Serialize() );
}

void
BrokerClient::HandlePubcomp( std::shared_ptr<PubcompPacket> apPacket )
{
   assertConnected();


}

void
BrokerClient::HandleSubscribe( std::shared_ptr<SubscribePacket> apPacket )
{
   assertConnected();
   std::vector<unsigned char> vecResponses;
   for( auto subReqs : apPacket->GetSubscribeRequests() )
   {
      vecResponses.push_back( subReqs.QOS );
   }

   SubackPacket pc( apPacket->GetPacketId(), vecResponses );
   m_pConnection->WriteAsync( pc.Serialize() );
}

void
BrokerClient::HandleSuback( std::shared_ptr<SubackPacket> apPacket )
{
   assertConnected();
}

void
BrokerClient::HandleUnsubscribe( std::shared_ptr<UnsubscribePacket> apPacket )
{
   assertConnected();

   m_pConnection->WriteAsync( UnsubackPacket( apPacket->GetPacketId() ).Serialize() );
}

void
BrokerClient::HandleUnsuback( std::shared_ptr<UnsubackPacket> apPacket )
{
   assertConnected();
}

void
BrokerClient::assertConnected()
{
   if( !m_pConnectPacket )
   {
      throw;
   }
}

}