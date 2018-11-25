#pragma once
#include "asio.hpp"
namespace me
{
class Broadcaster : public std::enable_shared_from_this<Broadcaster>
{
public:
   Broadcaster();
   ~Broadcaster();

   void BroadcastMessage()
};
}