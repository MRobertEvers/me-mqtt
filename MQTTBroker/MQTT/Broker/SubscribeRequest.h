#pragma once
#include "Definitions.h"
#include <string>

namespace me
{
struct SubscribeRequest
{
public:
   me::pcstring Topic;
   unsigned char QOS;

   SubscribeRequest( char const* apData, size_t aSize, unsigned char aQOS );
   SubscribeRequest( std::string aTopic, unsigned char aQOS );
};
}