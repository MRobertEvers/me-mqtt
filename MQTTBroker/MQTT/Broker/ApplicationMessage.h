#pragma once
#include <string>

class ApplicationMessage
{
public:
   ApplicationMessage(
      std::string const& aszTopic, std::string const& aszPayload,
      unsigned char aiQOS, bool abRetain );
   ~ApplicationMessage();

   std::string GetTopic() const;
   std::string GetPayload() const;
   unsigned char GetQOS() const;
   bool GetRetainFlag() const;

private:
   std::string m_szTopic;
   std::string m_szPayload;
   unsigned char m_iQOS;
   bool m_bRetain;
};

