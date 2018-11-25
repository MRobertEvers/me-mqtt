#include "stdafx.h"
#include "BroadcasterClient.h"
#include "Broadcaster.h"
#include "ApplicationMessage.h"

namespace me
{
BroadcasterClient::BroadcasterClient( 
   std::shared_ptr<Broadcaster> apBroadcaster )
   : m_pBroadcaster( apBroadcaster )
{
}

BroadcasterClient::~BroadcasterClient()
{

}

me::pcstring 
BroadcasterClient::GetClientName() const
{
   return m_szClientName;
}

void 
BroadcasterClient::SetClientName( me::pcstring apszName )
{
   m_szClientName = apszName;
}

void 
BroadcasterClient::BroadcastPublishMessage( 
   me::pcstring apszTopic, me::pcstring apszPayload, 
   unsigned char aiQOS, bool abRetain )
{
   auto pMessage = std::make_shared<ApplicationMessage>( 
      apszTopic, apszPayload, aiQOS, abRetain 
      );

   m_pBroadcaster->BroadcastMessage( pMessage );
}

}