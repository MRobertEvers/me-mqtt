#pragma once
#include "asio.hpp"
#include "RingBuffer.h"

class FixedHeader
{
public:
   FixedHeader();
   FixedHeader(unsigned char aPacketType, unsigned char aFlags, unsigned int aRemainingSize);

   ~FixedHeader();

   size_t FeedBytes( char* apBytes, size_t aNumBytes );

protected:
   size_t readBytes( RingBuffer& buf );
   void onReceiveBytes( const asio::error_code& aec, size_t aNumBytes );

private:
   unsigned char m_iPacketType;
   unsigned char m_iFlags;
   unsigned char m_iRemainingLength;

   char m_pBuf[5];
   char m_iCurrent;
   bool m_bDone;
};

