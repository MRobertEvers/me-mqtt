#pragma once
#include "asio.hpp"

class Broadcaster : public std::enable_shared_from_this<Broadcaster>
{
public:
   Broadcaster();
   ~Broadcaster();

   void BroadcastMessage()
};