#pragma once
#include <string>
class Broadcaster;
class BroadcasterClient
{
public:
   BroadcasterClient(std::string aszClientName, Broadcaster* apBroadcaster);
   ~BroadcasterClient();

   //void BroadcastPublishMessage()

private:
   std::string m_szClientName;
   Broadcaster* m_pBroadcaster;
};
