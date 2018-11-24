#pragma once
#include "ControlPacket.h"
#include <string>

class ConnectPacket :
   public ControlPacket
{
public:
   ConnectPacket( std::string& aszData, unsigned char aiFixedHeaderSize );
   ~ConnectPacket();

   //std::string_view GetProtocolName() const;
   //unsigned char GetProtocolLevel() const;
   //unsigned char GetConnectFlags() const;
   //unsigned short GetKeepAlive() const;

   //bool GetCleanSession() const;
   //bool GetWillPresent() const;
   //unsigned char GetWillQOS() const;
   //bool GetWillRetain() const;
   //bool GetUsernamePresent() const;
   //bool GetPasswordPresent() const;

   //std::string_view const GetClientName() const;
   //std::string_view const GetWillTopic() const;
   //std::string_view const GetWillPayload() const;
   //std::string_view const GetUsername() const;
   //std::string_view const GetPassword() const;

   //char const* GetVariableHeader() const;
   //char const* GetPayload() const;
private:
   
   //size_t m_iVariableHeaderStart;
   //size_t m_iPayloadStart;

   //std::string_view m_szClientName;
   //std::string_view m_szWillTopic;
   //std::string_view m_szWillPayload;
   //std::string_view m_szUsername;
   //std::string_view m_szPassword;
};

