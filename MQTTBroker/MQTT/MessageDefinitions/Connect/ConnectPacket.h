#pragma once
#include "ControlPacket.h"
#include "Definitions.h"
#include <string>

namespace me
{
class ConnectPacket :
   public ControlPacket
{
public:
   ConnectPacket( me::pcstring aszData, unsigned char aiFixedHeaderSize );
   ~ConnectPacket();

   me::pcstring GetProtocolName() const;
   unsigned char GetProtocolLevel() const;
   unsigned char GetConnectFlags() const;
   unsigned short GetKeepAlive() const;

   bool GetCleanSession() const;
   bool GetWillPresent() const;
   unsigned char GetWillQOS() const;
   bool GetWillRetain() const;
   bool GetUsernamePresent() const;
   bool GetPasswordPresent() const;

   me::pcstring const GetClientName() const;
   me::pcstring const GetWillTopic() const;
   me::pcstring const GetWillPayload() const;
   me::pcstring const GetUsername() const;
   me::pcstring const GetPassword() const;

   bool SetCleanSession( bool abCleanSession );
   bool SetWillPresent( bool abWillPresent );
   unsigned char SetWillQOS( unsigned char abWillQOS );
   bool SetWillRetain( bool abWillRetain );
   bool SetUsernamePresent( bool abUsernamePresent );
   bool SetPasswordPresent( bool abPasswordPresent );

private:
   unsigned char m_iFlags;
   unsigned char m_iProtocolLevel;
   unsigned short m_iKeepAlive;

   me::pcstring m_szProtocolName;

   me::pcstring m_szClientName;
   me::pcstring m_szWillTopic;
   me::pcstring m_szWillPayload;
   me::pcstring m_szUsername;
   me::pcstring m_szPassword;

   // Inherited via ControlPacket
   virtual std::string SerializeBody() const override;
};
}