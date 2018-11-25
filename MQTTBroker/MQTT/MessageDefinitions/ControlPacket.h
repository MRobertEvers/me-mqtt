#pragma once
#include "Definitions.h"
#include <string>
#include <memory>

namespace me
{

class ControlPacket
{
public:
   enum PacketTypes : unsigned char
   {
      RESERVED_0 = 0x00,
      CONNECT,
      CONNACK,
      PUBLISH,
      PUBACK,
      PUBREC,
      PUBREL,
      PUBCOMP,
      SUBSCRIBE,
      SUBACK,
      UNSUBSCRIBE,
      UNSUBACK,
      PINGREQ,
      PINGRESP,
      DISCONNECT,
      RESERVED_F
   };

public:
   PacketTypes GetType() const;

   std::string Serialize() const;

protected:
   // Leave reserved as 0x00 if overriding getFixedHeaderReserved
   ControlPacket( unsigned char aPacketType, unsigned char aiReserved );
   virtual ~ControlPacket();

   void setType( unsigned char aiType );
   virtual unsigned char getFixedHeaderReserved() const;

   virtual std::string SerializeBody() const = 0;
   virtual void SerializeLength( std::string& aszBuf, size_t aiLen ) const;

private:
   PacketTypes m_iPacketType;
   unsigned char m_iReserved;
};

}