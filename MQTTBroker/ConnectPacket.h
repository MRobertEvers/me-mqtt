#pragma once
#include "ControlPacket.h"
#include <string>


class ConnectPacket :
   public ControlPacket
{
public:
   ConnectPacket( std::string const& aszData, unsigned char aiFixedHeaderSize );
   ~ConnectPacket();

   std::string GetProtocolName() const;
   unsigned char GetProtocolLevel() const;
   unsigned char GetConnectFlags() const;
   unsigned short GetKeepAlive() const;

   bool GetCleanSession() const;
   bool GetWillPresent() const;
   unsigned char GetWillQOS() const;
   bool GetWillRetain() const;
   bool GetUsernamePresent() const;
   bool GetPasswordPresent() const;

   std::string const GetClientName() const;
   std::string const GetWillTopic() const;
   std::string const GetWillPayload() const;
   std::string const GetUsername() const;
   std::string const GetPassword() const;

   bool SetCleanSession(bool abCleanSession);
   bool SetWillPresent(bool abWillPresent);
   unsigned char SetWillQOS(unsigned char abWillQOS);
   bool SetWillRetain(bool abWillRetain);
   bool SetUsernamePresent(bool abUsernamePresent);
   bool SetPasswordPresent(bool abPasswordPresent);

private:
   unsigned char m_iFlags;
   unsigned char m_iProtocolLevel;
   unsigned short m_iKeepAlive;

   std::string m_szProtocolName;

   std::string m_szClientName;
   std::string m_szWillTopic;
   std::string m_szWillPayload;
   std::string m_szUsername;
   std::string m_szPassword;

   // Inherited via ControlPacket
   virtual std::string SerializeBody() const override;
};

