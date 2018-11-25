#include "stdafx.h"
#include "ApplicationMessage.h"
namespace me
{
ApplicationMessage::ApplicationMessage(
   me::pcstring aszTopic, me::pcstring aszPayload,
   unsigned char aiQOS, bool abRetain )
   : m_szTopic( aszTopic ), m_szPayload( aszPayload ), m_iQOS( aiQOS ), m_bRetain( abRetain )
{
}

ApplicationMessage::~ApplicationMessage()
{
}

me::pcstring
ApplicationMessage::GetTopic() const
{
   return m_szTopic;
}

me::pcstring
ApplicationMessage::GetPayload() const
{
   return m_szPayload;
}

unsigned char
ApplicationMessage::GetQOS() const
{
   return m_iQOS;
}

bool
ApplicationMessage::GetRetainFlag() const
{
   return m_bRetain;
}
}