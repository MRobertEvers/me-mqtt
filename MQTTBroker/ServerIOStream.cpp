#include "stdafx.h"
#include "ServerIOStream.h"


ServerIOStream::ServerIOStream()
{
}


ServerIOStream::~ServerIOStream()
{
}

ServerIOStream&
ServerIOStream::operator <<( std::ostream& (*var)(std::ostream&) )
{
   m_StreamLock.lock();
   std::cout << var;
   m_StreamLock.unlock();

   return *this;
}

void
ServerIOStream::Get()
{
   std::cin.get();
}
