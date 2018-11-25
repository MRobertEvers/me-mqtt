#include "stdafx.h"
#include "BroadcasterClient.h"
namespace me
{
BroadcasterClient::BroadcasterClient( std::string aszClientName, Broadcaster * apBroadcaster )
   : m_szClientName( aszClientName ), m_pBroadcaster( apBroadcaster )
{
}

BroadcasterClient::~BroadcasterClient()
{
}
}