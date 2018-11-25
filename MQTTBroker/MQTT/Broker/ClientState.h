#pragma once
#include "Definitions.h"
#include "ApplicationMessage.h"
#include <string>
#include <queue>
#include <memory>

namespace me
{
class BrokerClient;

class ClientState
{
public:
   ClientState( );
   ~ClientState();

   void SetWatcher( std::weak_ptr<BrokerClient> apSource );
   void DisconnectWatch();

   std::queue<std::shared_ptr<ApplicationMessage>>& GetPendingOutbound();
   void AddPendingOutbound( std::shared_ptr<ApplicationMessage> apMsg );

   std::queue<std::shared_ptr<ApplicationMessage>>& GetPendingPuback();
   void AddPendingPuback( std::shared_ptr<ApplicationMessage> apMsg );

   std::queue<std::shared_ptr<ApplicationMessage>>& GetPendingPubrec();
   void AddPendingPubrec( std::shared_ptr<ApplicationMessage> apMsg );

   std::queue<std::shared_ptr<ApplicationMessage>>& GetPendingPubcomp();
   void AddPendingPubcomp( std::shared_ptr<ApplicationMessage> apMsg );


private:
   // Messages waiting to be sent out.
   std::queue<std::shared_ptr<ApplicationMessage>> m_qPendingOutbound;

   // Sent QOS 1 messages that have not been puback'd by the client.
   std::queue<std::shared_ptr<ApplicationMessage>> m_qPendingPuback;

   // Sent QOS 2 message that have not been Pubrec'd by the client.
   std::queue<std::shared_ptr<ApplicationMessage>> m_qPendingPubrec;
   // Sent QOS 2 message that have been sent Pubrel but have not been Pubcomp'd by the client.
   std::queue<std::shared_ptr<ApplicationMessage>> m_qPendingPubcomp;

   std::weak_ptr<BrokerClient> m_pSource;
};
}