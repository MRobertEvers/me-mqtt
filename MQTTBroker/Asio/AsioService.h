#pragma once
#include "asio.hpp"
#include <thread>

class AsioService
{
public:
   AsioService(const int aiWorkerThreads);
   ~AsioService();

   const std::shared_ptr<asio::io_context> GetService() const;
   void Stop() const;

protected:
   void PWorkerThread();

private:
   std::shared_ptr<asio::io_context> m_pService;
   int m_iWorkerThread;
   std::thread** m_pThreadArr;
   std::shared_ptr<asio::executor_work_guard<asio::io_context::executor_type>> m_pWorkItem;
};

