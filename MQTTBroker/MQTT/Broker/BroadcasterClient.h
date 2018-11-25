#pragma once
#include "Definitions.h"
#include <string>

namespace me
{
class Broadcaster;
class BroadcasterClient
{
public:
   // TODO: This should be a shared_ptr?
   BroadcasterClient( std::shared_ptr<Broadcaster> pBroadcaster );
   ~BroadcasterClient();

   me::pcstring GetClientName() const;
   void SetClientName( me::pcstring apszName );

   void BroadcastPublishMessage( 
      me::pcstring apszTopic, me::pcstring apszPayload, 
      unsigned char aiQOS, bool abRetain );

private:
   me::pcstring m_szClientName;
   std::shared_ptr<Broadcaster> m_pBroadcaster;
};
}