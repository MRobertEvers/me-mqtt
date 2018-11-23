#include "stdafx.h"
#include "FixedHeader.h"


FixedHeader::FixedHeader()
   : m_iCurrent( 0 ), m_bDone(false)
{

}

FixedHeader::FixedHeader( unsigned char aPacketType, unsigned char aFlags, unsigned int aRemainingSize )
   : m_iPacketType( aPacketType ), m_iFlags( aFlags ), m_iRemainingLength( aRemainingSize )
{

}


FixedHeader::~FixedHeader()
{
}
