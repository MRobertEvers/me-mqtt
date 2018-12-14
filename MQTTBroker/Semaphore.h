#pragma once
#include <mutex>
#include <condition_variable>

namespace me
{

class Semaphore
{
public:
   Semaphore(unsigned int aiCount);
   ~Semaphore();

   void Notify();

   void Wait();

   void TryWait();

private:
   unsigned int m_iCount = 0;
   std::mutex m_Mutex;
   std::condition_variable m_CV;
};

}
