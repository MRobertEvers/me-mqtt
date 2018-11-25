#pragma once
#include "ControlPacketId.h"
namespace me
{
struct SubscribeRequest
{
   std::string Topic;
   unsigned char QOS;
   SubscribeRequest( char const* apData, size_t aSize, unsigned char aQOS )
   {
      Topic = std::string( apData, aSize );
      QOS = aQOS;
   }
   SubscribeRequest( std::string aTopic, unsigned char aQOS )
   {
      Topic = aTopic;
      QOS = aQOS;
   }
};

class SubscribePacket :
   public ControlPacketId
{
public:
   SubscribePacket( std::string const& aszData, unsigned char aiFixedHeaderSize );
   SubscribePacket( unsigned short aiPacketId );
   ~SubscribePacket();

   std::vector<SubscribeRequest> const& GetSubscribeRequests();

   // Inherited via ControlPacket
   virtual std::string SerializeBody() const override;

private:
   std::vector<SubscribeRequest> m_vecTopicRequests;
};
}