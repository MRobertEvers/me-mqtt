#include "stdafx.h"
#include "BrokerClient.h"
#include "AsioConnection.h"
#include "Connect\ConnectPacket.h"
#include "Connack\ConnackPacket.h"
#include "Broker/BroadcasterClient.h"
#include "PingResp\PingRespPacket.h"
#include "ApplicationMessage.h"
#include "BroadcasterClient.h"

namespace me
{

BrokerClient::BrokerClient(
   std::shared_ptr<ConnectPacket> apConnectPacket,
   std::shared_ptr<BroadcasterClient> apBroadcaster,
   AsioConnection* apConnection )
   : m_pConnection( apConnection ), m_pBroadcaster(apBroadcaster),
   m_pConnectPacket(apConnectPacket)
{

}

BrokerClient::~BrokerClient()
{
   // Unsubscribe to state; Actually we dont need to do 
   // this because the weak pointer will see
   // we have disconnected.
}

void 
BrokerClient::Accept( bool abSessionPresent )
{
   Respond( abSessionPresent, 0x00 );
}

void
BrokerClient::Reject( unsigned char aiReason )
{
   Respond( false, aiReason );
}

void
BrokerClient::Respond( bool abSessionPresent, unsigned char aiResponse )
{
   // Does not perform any validation of response.
   m_pConnection->WriteAsync( ConnackPacket( aiResponse, aiResponse ).Serialize() );
}

void
BrokerClient::Disconnect()
{
   m_pConnection->Stop();
}

me::pcstring
BrokerClient::GetClientName() const
{
   return m_pConnectPacket->GetClientName();
}



void 
BrokerClient::PublishTo( std::shared_ptr<ApplicationMessage> apMsg )
{
   m_pConnection->WriteAsync( PublishPacket(
      apMsg->GetTopic(), apMsg->GetPayload(), false, 0, false, 0
   ).Serialize() );
}

void
BrokerClient::HandleConnect( std::shared_ptr<ConnectPacket> apPacket )
{
   // If this is the same packet we were initialized with.
   if( m_pConnectPacket == apPacket ) 
   {
      // TODO: Validate packet.
      Accept(false);
      m_pBroadcaster->ConnectClient( shared_from_this() );
   }
   else
   {
      throw;
   }
}

void
BrokerClient::HandleConnack( std::shared_ptr<ConnackPacket> apPacket )
{

}

void
BrokerClient::HandlePingReq( std::shared_ptr<PingReqPacket> apPacket )
{
   m_pConnection->WriteAsync( PingRespPacket().Serialize() );
}

void
BrokerClient::HandlePingResp( std::shared_ptr<PingRespPacket> apPacket )
{

}

void
BrokerClient::HandleDisconnect( std::shared_ptr<DisconnectPacket> apPacket )
{
   m_pConnection->Stop();
}

void
BrokerClient::HandlePublish( std::shared_ptr<PublishPacket> apPacket )
{
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
}

void
BrokerClient::HandlePubrec( std::shared_ptr<PubrecPacket> apPacket )
{
   m_pConnection->WriteAsync( PubrelPacket( apPacket->GetPacketId() ).Serialize() );
}

void
BrokerClient::HandlePubrel( std::shared_ptr<PubrelPacket> apPacket )
{
   m_pConnection->WriteAsync( PubcompPacket( apPacket->GetPacketId() ).Serialize() );
}

void
BrokerClient::HandlePubcomp( std::shared_ptr<PubcompPacket> apPacket )
{
}

void
BrokerClient::HandleSubscribe( std::shared_ptr<SubscribePacket> apPacket )
{
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
}

void
BrokerClient::HandleUnsubscribe( std::shared_ptr<UnsubscribePacket> apPacket )
{
   m_pConnection->WriteAsync( UnsubackPacket( apPacket->GetPacketId() ).Serialize() );
}

void
BrokerClient::HandleUnsuback( std::shared_ptr<UnsubackPacket> apPacket )
{
}

}