#pragma once
#include "Definitions.h"
#include <string>

namespace me
{
class Broadcaster;
class BroadcasterClient
{
public:
   BroadcasterClient( me::pcstring aszClientName, Broadcaster* apBroadcaster );
   ~BroadcasterClient();

   void BroadcastPublishMessage( 
      me::pcstring apszTopic, me::pcstring apszPayload, 
      unsigned char aiQOS, bool abRetain );

private:
   me::pcstring m_szClientName;
   Broadcaster* m_pBroadcaster;
};
}