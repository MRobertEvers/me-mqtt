#pragma once
#include "ControlPacketId.h"
#include <vector>
namespace me
{
class SubackPacket :
   public ControlPacketId
{
public:
   SubackPacket( unsigned short aiPacketId, std::vector<unsigned char> avecResponses );
   ~SubackPacket();

   // Inherited via ControlPacket
   virtual std::string SerializeBody() const override;

private:
   std::vector<unsigned char> m_vecResponseCodes;
};
}