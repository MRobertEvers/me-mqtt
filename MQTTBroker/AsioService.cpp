#include "stdafx.h"
#include "AsioService.h"
#include <functional>

AsioService::AsioService( const int aiWorkerThreads )
   : m_pService( std::shared_ptr<asio::io_context>( new asio::io_context ) ),
   m_iWorkerThread(aiWorkerThreads),
   m_pWorkItem( std::shared_ptr<asio::executor_work_guard<asio::io_context::executor_type>>(
      new asio::executor_work_guard<asio::io_context::executor_type> (m_pService->get_executor())
      ) 
   )
{
   m_pThreadArr = new std::thread*[m_iWorkerThread];
   for( int i = 0; i < m_iWorkerThread; ++i )
   {
      m_pThreadArr[i] = new std::thread( std::bind( &AsioService::PWorkerThread, this ) );
   }
}


AsioService::~AsioService()
{
   for( int i = 0; i < m_iWorkerThread; ++i )
   {
      delete m_pThreadArr[i];
   }
   delete[] m_pThreadArr;
}

const std::shared_ptr<asio::io_service> 
AsioService::GetService() const
{
   return m_pService;
}

void
AsioService::Stop() const
{
   m_pWorkItem->reset();
   for( int i = 0; i < m_iWorkerThread; ++i )
   {
      m_pThreadArr[i]->join();
   }
}

void 
AsioService::PWorkerThread()
{
   m_pService->run();
}
