#pragma once
#include "ControlPacket.h"
namespace me
{
class PublishPacket :
   public ControlPacket
{
public:
   PublishPacket( std::string const& aszData, unsigned char aiFixedHeaderSize );
   ~PublishPacket();

   bool GetDuplicateFlag() const;
   unsigned char GetQOS() const;
   bool GetRetainFlag() const;
   std::string GetTopicName() const;
   unsigned short GetPacketId() const;
   std::string GetPayload() const;

protected:
   virtual unsigned char getFixedHeaderReserved() const override;

private:
   // Inherited via ControlPacket
   virtual std::string SerializeBody() const override;

   bool m_bDuplicateFlag;
   unsigned char m_iQOS;
   bool m_bRetainFlag;

   std::string m_szTopicName;
   unsigned short m_iPacketId;

   std::string m_szPayload;
};
}