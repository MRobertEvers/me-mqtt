#include "stdafx.h"
#include "Broadcaster.h"
#include "BroadcasterClient.h"
#include "BrokerClient.h"
#include "ClientState.h"
#include "ClientStateLedger.h"
#include "SubscriptionManager.h"
#include "RetainedTopicManager.h"
#include "RetainedTopic.h"
#include <iostream>


namespace me
{

Broadcaster::Broadcaster( std::shared_ptr<AsioService> apService )
   : m_pService(apService),
   m_pStrand( new asio::io_context::strand( *apService->GetService() ) )
{
   m_pRetainedTopicManager = std::make_shared<RetainedTopicManager>();
   m_pSubscriptionManager = std::make_shared<SubscriptionManager>();
   m_pWork =
      std::make_shared<asio::io_service::work>( *m_pService->GetService() );
   m_pClients = std::make_shared<ClientStateLedger>();
}

Broadcaster::~Broadcaster()
{
}

void
Broadcaster::Subscribe( 
   std::shared_ptr<ClientState> apClient,
   unsigned short aiRequestId,
   std::vector<SubscribeRequest> avecTopics )
{
   auto postCB =
      [this, self=shared_from_this(), 
      client=apClient, topics= avecTopics, aiRequestId]
   ()
   {
      self->subscribe( client, aiRequestId, topics );
   };

   asio::post(
      m_pService->GetService()->get_executor(),
      m_pStrand->wrap( postCB )
   );
}

void 
Broadcaster::Unsubscribe( 
   std::shared_ptr<ClientState> apClient,
   unsigned short aiRequestId,
   std::vector<me::pcstring> avecTopics )
{
   auto postCB =
      [this, self = shared_from_this(), 
      aiRequestId, client = apClient, topics = avecTopics]
   ()
   {
      self->unsubscribe( client, aiRequestId, topics );
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

bool
Broadcaster::ClientExists( me::pcstring apClientName )
{
   auto pExistState = m_pClients->FindClient( apClientName );
   return pExistState != nullptr;
}

std::shared_ptr<ClientState>
me::Broadcaster::ConnectClient( std::weak_ptr<BroadcasterClient> apClient )
{
   // This needs to be synchronized.
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
Broadcaster::DisconnectClient( std::weak_ptr<BroadcasterClient> apClient )
{
   // This needs to be synchronized.
   auto pDisconnect =
      [this, self = shared_from_this(), pClient = apClient.lock()]
   ()
   {
      self->disconnect( pClient );
   };

   asio::post(
      m_pService->GetService()->get_executor(),
      m_pStrand->wrap( pDisconnect )
   );
}

void 
Broadcaster::subscribe( 
   std::shared_ptr<ClientState> apClient,
   unsigned short aiRequestId,
   std::vector<SubscribeRequest> avecTopics )
{
   // I have a topic filter. I want all topic that match that filter with 
   // retained messages.
   // And register myself as listening to that topic.
   // Get Retained messages
   // Subscribe

      // TODO: Get result from subscribe, then notify client
   m_pSubscriptionManager->Subscribe( apClient, aiRequestId, avecTopics );

   std::vector<std::shared_ptr<RetainedTopic>> vecMsgs;
   for( auto topic : avecTopics )
   {
      auto d = m_pRetainedTopicManager->GetRetainedMessages( topic.Topic );
      vecMsgs.insert( vecMsgs.begin(), d.begin(), d.end() );
   }

   for( auto topic : vecMsgs )
   {
      for( auto msg : topic->GetMessages() )
      {
         auto pOutbound = std::make_shared<ApplicationMessage>(
            msg->GetTopic(), msg->GetPayload(),
            msg->GetQOS(), true );
         apClient->AddPendingOutbound( pOutbound );
      }
   }
}

void 
Broadcaster::unsubscribe( 
   std::shared_ptr<ClientState> apClient,
   unsigned short aiRequestId, 
   std::vector<me::pcstring> avecTopics )
{
   // I have a topic filter. I want to find the topic filter that
   // matches exactly.

   m_pSubscriptionManager->Unsubscribe( apClient, aiRequestId, avecTopics );
   
}

void
Broadcaster::broadcast( std::shared_ptr<ApplicationMessage> apMessage )
{
   // This is only for NON-RETAINED MESSAGES
   // All messages through here are a result of being subscribed to a topic.
   // I have a topic name. I want to find all clients subscribed to
   // matching filter
   if( apMessage->GetRetainFlag() )
   {
      m_pRetainedTopicManager->RetainMessage( apMessage );
   }

   auto subs = m_pSubscriptionManager->GetSubscriptions( 
      apMessage->GetTopic() 
   );
   for( auto sub : subs )
   {
      auto subers = sub->GetSubscribers();
      for( auto suber : subers )
      {
         auto maxQOS = suber.second;
         std::shared_ptr<ApplicationMessage> pOutbound = apMessage;
         if( maxQOS < apMessage->GetQOS() )
         {
            pOutbound = std::make_shared<ApplicationMessage>(
               pOutbound->GetTopic(), pOutbound->GetPayload(),
               maxQOS, false );
         }
         if( pOutbound->GetRetainFlag() )
         {
            pOutbound = std::make_shared<ApplicationMessage>(
               pOutbound->GetTopic(), pOutbound->GetPayload(),
               pOutbound->GetQOS(), false );
         }
         suber.first->AddPendingOutbound( pOutbound );
      }
   }
}

void
Broadcaster::disconnect( std::weak_ptr<BroadcasterClient> apClient )
{
   auto pszClient = apClient.lock()->GetClientName();
   m_pClients->DeleteClient( pszClient );
}

}