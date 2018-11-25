#pragma once
#include "AsioService.h"
#include "ApplicationMessage.h"
#include "Definitions.h"

namespace me
{
class BroadcasterClient;

class Broadcaster : public std::enable_shared_from_this<Broadcaster>
{
public:
   Broadcaster( std::shared_ptr<AsioService> apIOService );
   ~Broadcaster();

   void BroadcastMessage( std::shared_ptr<ApplicationMessage> apMessage );
   std::shared_ptr<BroadcasterClient> CreateClient();

private:
   void broadcast( std::shared_ptr<ApplicationMessage> apMessage );

   std::shared_ptr<AsioService> m_pService;
   std::shared_ptr<asio::io_service::work> m_pWork;
};
}