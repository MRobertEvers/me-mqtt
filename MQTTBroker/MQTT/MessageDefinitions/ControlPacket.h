#pragma once
#include <string>


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
   ControlPacket( unsigned char aPacketType );
   virtual ~ControlPacket();

   void setType( unsigned char aiType );

   virtual std::string SerializeBody() const = 0;

private:
   PacketTypes m_iPacketType;
};
