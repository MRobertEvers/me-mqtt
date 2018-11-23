#pragma once

class RingBuffer
{
public:
   RingBuffer(size_t aiSize);
   ~RingBuffer();

   void Push( const char* apBytes, size_t aiSize );
   size_t Read( char * rpBytes, size_t aiRead );

   void Flush();

   size_t Size() const;

private:
   char* m_pBuf;
   size_t m_iHead;
   size_t m_iNext;
   size_t m_iSize;
   bool m_bHeadPush;
};

