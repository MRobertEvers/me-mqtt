#pragma once
#include "ControlPacketId.h"
#include <vector>
namespace me
{
class UnsubscribePacket :
   public ControlPacketId
{
public:
   UnsubscribePacket( std::string const& aszData, unsigned char aiFixedHeaderSize );
   UnsubscribePacket( unsigned short aiPacketId );
   ~UnsubscribePacket();

   // Inherited via ControlPacket
   virtual std::string SerializeBody() const override;
private:
   std::vector<std::string> m_vecUnsubRequests;
};
}