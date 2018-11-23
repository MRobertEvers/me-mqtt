#include "stdafx.h"
#include "RingBuffer.h"


RingBuffer::RingBuffer(size_t aiSize)
   : m_pBuf( new char[aiSize] ), m_iNext(0), m_iHead(0), m_iSize(aiSize)
{
}


RingBuffer::~RingBuffer()
{
   delete[] m_pBuf;
}

void 
RingBuffer::Push( const char * apBytes, size_t aiSize )
{
   size_t offset = 0;
   size_t remaining = 0;
   size_t toCopy = 0;

   m_bHeadPush = m_iSize - Size() < aiSize;
   while( offset != aiSize )
   {
      remaining = (aiSize - offset);
      if( m_iHead + remaining > m_iSize )
      {
         toCopy = m_iSize - m_iHead;
      }
      else
      {
         toCopy = remaining;
      }
      // Just copy it.
      memcpy_s( m_pBuf + m_iHead, m_iSize - m_iHead, apBytes+offset, toCopy );
      offset += toCopy;
      m_iHead = (m_iHead+toCopy)%m_iSize;
   }

   if( m_bHeadPush )
   {
      m_iNext = m_iHead;
   }
}

size_t
RingBuffer::Read( char* rpBytes, size_t aiRead )
{
   // TODO: Mutex
   size_t iRead = 0;
   
   size_t toRead = 0;
   size_t iSize = Size();
   if( iSize < aiRead )
   {
      toRead = iSize;
   }
   else
   {
      toRead = aiRead;
   }

   size_t thisRead = 0;
   while( iRead != aiRead )
   {
      if( m_iHead > m_iNext )
      {
         if( m_iHead - m_iNext <= toRead )
         {
            thisRead = m_iHead - m_iNext;
         }
         else
         {
            thisRead = toRead;
         }
      }
      else
      {
         if( m_iSize - m_iNext <= toRead )
         {
            thisRead = m_iSize - m_iNext;
         }
         else
         {
            thisRead = toRead;
         }
         m_bHeadPush = false;
      }

      memcpy_s( rpBytes+iRead, aiRead, m_pBuf+m_iNext, thisRead );
      m_iNext = (m_iNext + thisRead) % m_iSize;
      iRead += thisRead;
   }

   return iRead;
}

void 
RingBuffer::Flush()
{
}

size_t 
RingBuffer::Size() const
{
   if( m_iHead > m_iNext )
   {
      return m_iHead - m_iNext;
   }
   else if( m_iHead < m_iNext )
   {
      return m_iSize - m_iNext + m_iHead;
   }
   else if( m_bHeadPush )
   {
      return m_iSize;
   }
   else
   {
      return 0;
   }
}
