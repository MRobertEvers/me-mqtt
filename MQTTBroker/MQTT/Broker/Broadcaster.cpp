#include "stdafx.h"
#include "Broadcaster.h"
#include "BroadcasterClient.h"
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
Broadcaster::BroadcastMessage( 
   std::shared_ptr<ApplicationMessage> apMessage )
{
   asio::post( 
      m_pService->GetService()->get_executor(),
      [this, self = shared_from_this(), msg=apMessage]()->void{
      self->broadcast( msg );
   });
}

std::shared_ptr<BroadcasterClient>
Broadcaster::CreateClient()
{
   return std::make_shared<BroadcasterClient>( shared_from_this() );
}

void 
Broadcaster::broadcast( std::shared_ptr<ApplicationMessage> apMessage )
{
   std::cout << "Published" << *apMessage->GetPayload() << std::endl;
}

}