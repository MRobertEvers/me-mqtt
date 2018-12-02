#include "stdafx.h"
#include "Topic.h"
namespace me
{

Topic::Topic( me::pcstring apszSource )
   : m_pszFilter( apszSource )
{
}
Topic::~Topic()
{
}

me::pcstring 
Topic::GetFilter() const
{
   return m_pszFilter;
}


utils::pcview
Topic::PeekLevel( unsigned int aiLevel ) const
{
   if( aiLevel == 0 || aiLevel > Levels() )
   {
      return utils::pcview();
   }
   int i = 0;
   int iOff = 0;
   int iEnd = -1;
   while( i < aiLevel )
   {
      size_t iNextOff = m_pszFilter->find( '/', iEnd + 1 );
      if( iNextOff != std::string::npos )
      {
         iOff = iEnd + 1;
         iEnd = iNextOff;
      }
      else
      {
         iOff = iEnd + 1;
         iEnd = m_pszFilter->size();
      }
      i++;
   }

   return utils::pcview( m_pszFilter, m_pszFilter->data() + iOff, iEnd - iOff );
}
size_t
Topic::Levels() const
{
   return 1 + std::count( m_pszFilter->begin(), m_pszFilter->end(), '/' );
}
}
