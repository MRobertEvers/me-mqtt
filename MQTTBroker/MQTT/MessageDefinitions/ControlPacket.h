#pragma once
#include <string>

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
   ControlPacket( unsigned char aPacketType, unsigned char aiReserved );
   virtual ~ControlPacket();

   void setType( unsigned char aiType );
   virtual unsigned char getFixedHeaderReserved() const;

   virtual std::string SerializeBody() const = 0;

private:
   PacketTypes m_iPacketType;
   unsigned char m_iReserved;
};

}