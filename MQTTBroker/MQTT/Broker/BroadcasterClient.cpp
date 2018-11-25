#include "stdafx.h"
#include "BroadcasterClient.h"

BroadcasterClient::BroadcasterClient( std::string aszClientName, Broadcaster * apBroadcaster )
   : m_szClientName(aszClientName), m_pBroadcaster(apBroadcaster)
{
}

BroadcasterClient::~BroadcasterClient()
{
}
