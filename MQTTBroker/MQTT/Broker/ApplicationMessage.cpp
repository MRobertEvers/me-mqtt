#include "stdafx.h"
#include "ApplicationMessage.h"
namespace me
{
ApplicationMessage::ApplicationMessage(
   std::string const & aszTopic, std::string const & aszPayload,
   unsigned char aiQOS, bool abRetain )
   : m_szTopic( aszTopic ), m_szPayload( aszPayload ), m_iQOS( aiQOS ), m_bRetain( abRetain )
{
}

ApplicationMessage::~ApplicationMessage()
{
}

std::string
ApplicationMessage::GetTopic() const
{
   return m_szTopic;
}

std::string
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