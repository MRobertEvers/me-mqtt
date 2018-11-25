#include "stdafx.h"
#include "BroadcasterClient.h"
#include "Broadcaster.h"
#include "ApplicationMessage.h"

namespace me
{
BroadcasterClient::BroadcasterClient( me::pcstring aszClientName, Broadcaster * apBroadcaster )
   : m_szClientName( aszClientName ), m_pBroadcaster( apBroadcaster )
{
}

BroadcasterClient::~BroadcasterClient()
{

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