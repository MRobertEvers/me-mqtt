#pragma once
#include "Definitions.h"
#include <string>

namespace me
{
// Tracks all the clients subsribed to this filter.
class Subscription
{
public:
   Subscription( );
   ~Subscription();

private:
   me::pcstring m_szTopicFilter;
};

}