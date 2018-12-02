#pragma once
#include "Definitions.h"
#include "Utils.h"
#include <string>

namespace me
{
// Stores retained messages.
class Topic
{
public:
   Topic( me::pcstring apszSource );
   ~Topic();
   me::pcstring GetFilter() const;
   utils::pcview PeekLevel( unsigned int aiLevel ) const;
   size_t Levels() const;
private:
   me::pcstring m_pszFilter;
};

}