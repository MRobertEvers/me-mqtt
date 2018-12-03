#include "stdafx.h"
#include "BrokerClient.h"
#include "AsioConnection.h"
#include "Connect\ConnectPacket.h"
#include "Connack\ConnackPacket.h"
#include "Broker/BroadcasterClient.h"
#include "PingResp\PingRespPacket.h"
#include "ClientState.h"
#include "asio.hpp"
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
   if( m_pConnectPacket->GetWillPresent() )
   {
      m_pWillMessage = std::make_shared<ApplicationMessage>(
         m_pConnectPacket->GetWillTopic(), m_pConnectPacket->GetWillPayload(),
         m_pConnectPacket->GetWillQOS(), m_pConnectPacket->GetWillRetain()
         );
   }
}

BrokerClient::~BrokerClient()
{
   // Unsubscribe to state; Actually we dont need to do 
   // this because the weak pointer will see
   // we have disconnected.
   m_pBroadcaster->DisconnectClient( !m_pConnectPacket->GetCleanSession() );
   if( m_pWillMessage )
   {
      m_pBroadcaster->BroadcastPublishMessage( m_pWillMessage );
   }
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
BrokerClient::Respond( 
   bool abSessionPresent, unsigned char aiResponse )
{
   // Does not perform any validation of response.
   m_pConnection->WriteAsync( 
      ConnackPacket( aiResponse, aiResponse ).Serialize() );
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
   publishTo( apMsg );
}

void
BrokerClient::NotifyUnsubscribed( unsigned short aiRequestId )
{
   notifyUnsubscribed( aiRequestId );
}

void
BrokerClient::NotifySubscribed(
   unsigned short aiRequestId, 
   std::vector<unsigned char> avecResponses )
{
   notifySubscribed( aiRequestId, avecResponses );
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
   m_pConnection->Stop();
}

void
BrokerClient::HandlePingReq( std::shared_ptr<PingReqPacket> apPacket )
{
   m_pConnection->WriteAsync( PingRespPacket().Serialize() );
}

void
BrokerClient::HandlePingResp( std::shared_ptr<PingRespPacket> apPacket )
{
   m_pConnection->Stop();
}

void
BrokerClient::HandleDisconnect( std::shared_ptr<DisconnectPacket> apPacket )
{
   // Delete will.
   m_pWillMessage = nullptr;
   m_pConnection->Stop();
}

void
BrokerClient::HandlePublish( std::shared_ptr<PublishPacket> apPacket )
{
   auto pMessage = std::make_shared<ApplicationMessage>(
      apPacket->GetTopicName(), apPacket->GetPayload(),
      apPacket->GetQOS(), apPacket->GetRetainFlag()
      );
   
   unsigned char iQos = apPacket->GetQOS();
   if( iQos == 1 )
   {
      m_pConnection->WriteAsync(
         PubackPacket( apPacket->GetPacketId() ).Serialize() );
   }
   else if( iQos == 2 )
   {
      auto cState = m_pBroadcaster->GetState();
      cState->AddPendingPubrel( apPacket->GetPacketId(), pMessage );
      m_pConnection->WriteAsync(
         PubrecPacket( apPacket->GetPacketId() ).Serialize() );
   }

   if( iQos < 2 )
   {
      m_pBroadcaster->BroadcastPublishMessage(
         pMessage
      );
   }
}

void
BrokerClient::HandlePuback( std::shared_ptr<PubackPacket> apPacket )
{
   auto cState = m_pBroadcaster->GetState();
   auto msg = cState->ReleasePendingPuback( apPacket->GetPacketId() );
   if( msg )
   {

   }
   else
   {
      // TODO: Stray puback?
   }
}

void
BrokerClient::HandlePubrec( std::shared_ptr<PubrecPacket> apPacket )
{
   auto cState = m_pBroadcaster->GetState();
   auto msg = cState->ReleasePendingPubrec( apPacket->GetPacketId() );
   if( msg )
   {
      cState->AddPendingPubcomp( apPacket->GetPacketId(), msg );
      m_pConnection->WriteAsync( 
         PubrelPacket( apPacket->GetPacketId() ).Serialize() );
   }
   else
   {
      // TODO: Stray pubrec?
   }
}

void
BrokerClient::HandlePubrel( std::shared_ptr<PubrelPacket> apPacket )
{
   auto cState = m_pBroadcaster->GetState();
   auto msg = cState->ReleasePendingPubrel( apPacket->GetPacketId() );
   if( msg )
   {
      m_pConnection->WriteAsync( 
         PubcompPacket( apPacket->GetPacketId() ).Serialize() );

      m_pBroadcaster->BroadcastPublishMessage(
         msg
      );
   }
   else
   {
      // TODO: Stray pubrel?
   }
   // We sent a pubrec, this is the client's response.
}

void
BrokerClient::HandlePubcomp( std::shared_ptr<PubcompPacket> apPacket )
{
   auto cState = m_pBroadcaster->GetState();
   auto msg = cState->ReleasePendingPubcomp( apPacket->GetPacketId() );
   if( msg )
   {

   }
   else
   {
      // TODO: Stray pubcomp?
   }
}

void
BrokerClient::HandleSubscribe( std::shared_ptr<SubscribePacket> apPacket )
{
   m_pBroadcaster->SubscribeToTopics( 
      apPacket->GetPacketId(), 
      apPacket->GetSubscribeRequests() );
}

void
BrokerClient::HandleSuback( std::shared_ptr<SubackPacket> apPacket )
{
   m_pConnection->Stop();
}

void
BrokerClient::HandleUnsubscribe( std::shared_ptr<UnsubscribePacket> apPacket )
{
   m_pBroadcaster->UnsubscribeFromTopics( 
      apPacket->GetPacketId(), 
      apPacket->GetUnsubscribeRequests() );
}

void
BrokerClient::HandleUnsuback( std::shared_ptr<UnsubackPacket> apPacket )
{
   m_pConnection->Stop();
}

void 
BrokerClient::publishTo( std::shared_ptr<ApplicationMessage> apMsg )
{
   auto pPublish = 
      [this, self=shared_from_this(), msg = apMsg]()
   {
      unsigned short aiPacketId = m_pBroadcaster->GetState()->GetNewPacketId();

      // TODO retry if message no acked.
      m_pConnection->WriteAsync( PublishPacket(
         msg->GetTopic(), msg->GetPayload(), false,
         msg->GetQOS(), msg->GetRetainFlag(), aiPacketId
      ).Serialize() );

      auto qos = msg->GetQOS();
      if( qos == 1 )
      {
         auto cState = m_pBroadcaster->GetState();
         cState->AddPendingPuback( aiPacketId, msg );
      }
      else if( qos == 2 )
      {
         auto cState = m_pBroadcaster->GetState();
         cState->AddPendingPubrec( aiPacketId, msg );
      }
   };

   asio::post(
      m_pConnection->GetStrand()->get_io_context().get_executor(),
      m_pConnection->GetStrand()->wrap( pPublish )
   );

}

void 
BrokerClient::notifyUnsubscribed(unsigned short aiPacketId)
{
   auto pUnsubscribe =
      [this, self = shared_from_this(), aiPacketId]()
   {
      m_pConnection->WriteAsync(
         UnsubackPacket( aiPacketId ).Serialize() );
   };

   asio::post(
      m_pConnection->GetStrand()->get_io_context().get_executor(),
      m_pConnection->GetStrand()->wrap( pUnsubscribe )
   );
}

void 
BrokerClient::notifySubscribed(
   unsigned short aiPacketId, std::vector<unsigned char> avecResponses )
{
   auto pSubscribe =
      [this, self = shared_from_this(), aiPacketId, avecResponses]()
   {
      std::vector<unsigned char> vecResponses;
      for( auto subReqs : avecResponses )
      {
         vecResponses.push_back( subReqs );
      }

      SubackPacket pc( aiPacketId, vecResponses );
      m_pConnection->WriteAsync( pc.Serialize() );
   };

   asio::post(
      m_pConnection->GetStrand()->get_io_context().get_executor(),
      m_pConnection->GetStrand()->wrap( pSubscribe )
   );
}

}