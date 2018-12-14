#include "stdafx.h"
#include "Semaphore.h"
namespace me
{

Semaphore::Semaphore( unsigned int aiCount )
   : m_iCount(aiCount)
{
}


Semaphore::~Semaphore()
{
}

void Semaphore::Notify()
{
   std::unique_lock<std::mutex> lk( m_Mutex );
   ++m_iCount;
   lk.unlock();
   m_CV.notify_one();
}

void Semaphore::Wait()
{
   std::unique_lock<std::mutex> lk( m_Mutex );
   while( m_iCount == 0 )
   {
      m_CV.wait(lk);
   }
   --m_iCount;
}

void Semaphore::TryWait()
{
}

}