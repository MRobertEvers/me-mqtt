#pragma once
#include "ControlPacketId.h"
#include "Definitions.h"

namespace me
{
struct SubscribeRequest
{
   me::pcstring Topic;
   unsigned char QOS;
   SubscribeRequest( char const* apData, size_t aSize, unsigned char aQOS )
   {
      Topic = std::make_shared<std::string>( apData, aSize );
      QOS = aQOS;
   }
   SubscribeRequest( std::string aTopic, unsigned char aQOS )
   {
      Topic = std::make_shared<std::string>(aTopic);
      QOS = aQOS;
   }
};

class SubscribePacket :
   public ControlPacketId
{
public:
   SubscribePacket( me::pcstring aszData, unsigned char aiFixedHeaderSize );
   SubscribePacket( unsigned short aiPacketId );
   ~SubscribePacket();

   std::vector<SubscribeRequest> const& GetSubscribeRequests();

   // Inherited via ControlPacket
   virtual std::string SerializeBody() const override;

private:
   std::vector<SubscribeRequest> m_vecTopicRequests;
};
}