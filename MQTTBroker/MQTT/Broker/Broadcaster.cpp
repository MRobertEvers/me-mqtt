#include "stdafx.h"
#include "Broadcaster.h"
#include "BroadcasterClient.h"
#include "BrokerClient.h"
#include "ClientState.h"
#include "ClientStateLedger.h"
#include "SubscriptionManager.h"
#include "TopicManager.h"
#include <iostream>


namespace me
{

Broadcaster::Broadcaster( std::shared_ptr<AsioService> apService )
   : m_pService(apService),
   m_pStrand( new asio::io_context::strand( *apService->GetService() ) )
{
   m_pSubscriptionManager = std::make_shared<SubscriptionManager>();
   m_pTopicManager = std::make_shared<TopicManager>();
   m_pWork =
      std::make_shared<asio::io_service::work>( *m_pService->GetService() );
   m_pClients = std::make_shared<ClientStateLedger>();
}

Broadcaster::~Broadcaster()
{
}

void
Broadcaster::Subscribe( std::shared_ptr<ClientState> apClient, me::pcstring apszTopic )
{
   auto postCB =
      [this, self=shared_from_this(), client= apClient, topic=apszTopic]
   ()
   {
      self->subscribe( client, topic );
   };

   asio::post(
      m_pService->GetService()->get_executor(),
      m_pStrand->wrap( postCB )
   );
}

void 
Broadcaster::Unsubscribe( std::shared_ptr<ClientState> apClient, me::pcstring apszTopic )
{
   auto postCB =
      [this, self = shared_from_this(), client = apClient, topic = apszTopic]
   ()
   {
      self->unsubscribe( client, topic );
   };

   asio::post(
      m_pService->GetService()->get_executor(),
      m_pStrand->wrap( postCB )
   );
}

void
Broadcaster::BroadcastMessage( 
   std::shared_ptr<ApplicationMessage> apMessage )
{
   auto postCB = 
      [this, self = shared_from_this(), msg = apMessage]
   ()
   {
      self->broadcast( msg );
   };

   asio::post(
      m_pService->GetService()->get_executor(),
      m_pStrand->wrap( postCB )
   );
}

std::shared_ptr<BroadcasterClient>
Broadcaster::CreateClient()
{
   return std::make_shared<BroadcasterClient>( shared_from_this() );
}

std::shared_ptr<ClientState>
me::Broadcaster::ConnectClient( std::weak_ptr<BroadcasterClient> apClient )
{
   auto pClient = apClient.lock()->GetClient().lock();

   auto pExistState = m_pClients->FindClient( pClient->GetClientName() );
   if( pExistState )
   {
      pExistState->DisconnectWatch();
   }
   else
   {
      pExistState = m_pClients->CreateClient( pClient->GetClientName() );
   }

   pExistState->SetWatcher( pClient );
   return pExistState;
}

void 
Broadcaster::subscribe( std::shared_ptr<ClientState> apClient, me::pcstring apszTopic )
{
   // I have a topic filter. I want all topic that match that filter with retained messages.
   // And register myself as listening to that topic.
   // Get Retained messages
   // Subscribe
   m_pSubscriptionManager->Subscribe( apClient, apszTopic );
}

void 
Broadcaster::unsubscribe( std::shared_ptr<ClientState> apClient, me::pcstring apszTopic )
{
   // I have a topic filter. I want to find the topic filter that matches exactly.
   m_pSubscriptionManager->Unsubscribe( apClient, apszTopic );
}

void
Broadcaster::broadcast( std::shared_ptr<ApplicationMessage> apMessage )
{
   // I have a topic name. I want to find all clients subscribed toa  matching filter
   auto subs = m_pSubscriptionManager->GetSubscriptions( apMessage->GetTopic() );
   for( auto sub : subs )
   {
      auto subers = sub->GetSubscribers();
      for( auto suber : subers )
      {
         suber->AddPendingOutbound( apMessage );
      }
   }
}

}