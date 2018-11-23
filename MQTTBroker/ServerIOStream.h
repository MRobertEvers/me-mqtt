#pragma once
#include <mutex>
#include <iostream>

class ServerIOStream
{
public:
   ServerIOStream();
   ~ServerIOStream();

   template<typename T>
   ServerIOStream& operator <<( T item );
   ServerIOStream& operator <<( std::ostream& (*var)(std::ostream&) ); // For std::endl

   void Get();

private:
   std::mutex m_StreamLock;
};

template<typename T>
ServerIOStream& ServerIOStream::operator <<( T item )
{
   m_StreamLock.lock();
   std::cout << item;
   m_StreamLock.unlock();

   return *this;
}
