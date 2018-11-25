#pragma once
#include "Definitions.h"
#include <string>

namespace me
{
class Broadcaster;
class BroadcasterClient
{
public:
   BroadcasterClient( Broadcaster* apBroadcaster );
   ~BroadcasterClient();

   me::pcstring GetClientName() const;
   void SetClientName( me::pcstring apszName );

   void BroadcastPublishMessage( 
      me::pcstring apszTopic, me::pcstring apszPayload, 
      unsigned char aiQOS, bool abRetain );

private:
   me::pcstring m_szClientName;
   Broadcaster* m_pBroadcaster;
};
}