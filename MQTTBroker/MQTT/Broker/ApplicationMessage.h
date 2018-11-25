#pragma once
#include "Definitions.h"
#include <string>

namespace me
{
class ApplicationMessage
{
public:
   ApplicationMessage(
      me::pcstring aszTopic, me::pcstring aszPayload,
      unsigned char aiQOS, bool abRetain );
   ~ApplicationMessage();

   me::pcstring GetTopic() const;
   me::pcstring GetPayload() const;
   unsigned char GetQOS() const;
   bool GetRetainFlag() const;

private:
   me::pcstring m_szTopic;
   me::pcstring m_szPayload;
   unsigned char m_iQOS;
   bool m_bRetain;
};

}