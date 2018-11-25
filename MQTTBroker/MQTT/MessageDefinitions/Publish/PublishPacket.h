#pragma once
#include "ControlPacket.h"
namespace me
{
class PublishPacket :
   public ControlPacket
{
public:
   PublishPacket( me::pcstring aszData, unsigned char aiFixedHeaderSize );
   ~PublishPacket();

   bool GetDuplicateFlag() const;
   unsigned char GetQOS() const;
   bool GetRetainFlag() const;
   me::pcstring GetTopicName() const;
   unsigned short GetPacketId() const;
   me::pcstring GetPayload() const;

protected:
   virtual unsigned char getFixedHeaderReserved() const override;

private:
   // Inherited via ControlPacket
   virtual std::string SerializeBody() const override;

   bool m_bDuplicateFlag;
   unsigned char m_iQOS;
   bool m_bRetainFlag;

   me::pcstring m_szTopicName;
   unsigned short m_iPacketId;

   me::pcstring m_szPayload;
};
}