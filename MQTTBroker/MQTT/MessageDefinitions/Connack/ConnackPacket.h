#pragma once
#include "ControlPacket.h"

class ConnackPacket: public ControlPacket
{
public:
   enum ReturnCodes : unsigned char
   {
      ACCEPTED = 0x00,
      UNACCEPTABLE_PROTOCOL_VERSION,
      IDENTIFIER_REJECTED,
      SERVER_UNAVAILABLE,
      BAD_USERNAME_OR_PASSWORD,
      NOT_AUTHORIZED
   };

public:
   ConnackPacket(bool abSessionPresent, unsigned char abCode);
   ~ConnackPacket();

   bool GetSessionPresent() const;
   ReturnCodes GetReturnCode() const;

   // Inherited via ControlPacket
   virtual std::string SerializeBody() const override;
private:
   bool m_bSessionPresent;
   ReturnCodes m_iReturnCode;

};

