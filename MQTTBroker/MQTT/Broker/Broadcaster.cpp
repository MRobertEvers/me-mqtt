#include "stdafx.h"
#include "Broadcaster.h"
#include "BroadcasterClient.h"
#include "BrokerClient.h"
#include "ClientState.h"
#include "ClientStateLedger.h"
#include <iostream>


namespace me
{

Broadcaster::Broadcaster( std::shared_ptr<AsioService> apService )
   : m_pService(apService),
   m_pStrand( new asio::io_context::strand( *apService->GetService() ) )
{
   m_pWork =
      std::make_shared<asio::io_service::work>( *m_pService->GetService() );
   m_pClients = std::make_shared<ClientStateLedger>();
}

Broadcaster::~Broadcaster()
{
}

void
Broadcaster::Subscribe( me::pcstring apszClient, me::pcstring apszTopic )
{
   auto postCB =
      [this, self=shared_from_this(), client=apszClient, topic=apszTopic]
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

void 
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
}

void 
Broadcaster::subscribe( me::pcstring apszClient, me::pcstring apszTopic )
{

}

void
Broadcaster::broadcast( std::shared_ptr<ApplicationMessage> apMessage )
{
   auto p = std::make_shared<std::string>( "MQTT_FX_Client" );
   auto pExistState = m_pClients->FindClient( p );
   if( pExistState )
   {
      pExistState->AddPendingOutbound(apMessage);
   }
}

}