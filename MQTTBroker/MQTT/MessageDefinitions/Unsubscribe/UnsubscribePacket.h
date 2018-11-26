#pragma once
#include "ControlPacketId.h"
#include "Definitions.h"
#include <vector>

namespace me
{
class UnsubscribePacket :
   public ControlPacketId
{
public:
   UnsubscribePacket( me::pcstring aszData, unsigned char aiFixedHeaderSize );
   UnsubscribePacket( unsigned short aiPacketId );
   ~UnsubscribePacket();

   std::vector<me::pcstring> GetUnsubscribeRequests() const;

   // Inherited via ControlPacket
   virtual std::string SerializeBody() const override;
private:
   std::vector<me::pcstring> m_vecUnsubRequests;
};
}