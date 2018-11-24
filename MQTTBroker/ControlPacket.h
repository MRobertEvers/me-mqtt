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
   ControlPacket( std::string aszData );
   virtual ~ControlPacket();

   PacketTypes GetType();
   size_t GetLength();

protected:
   char const* data() const noexcept;
   size_t size() const noexcept;

private:
   std::string m_szData;
   size_t m_iLength;
};

