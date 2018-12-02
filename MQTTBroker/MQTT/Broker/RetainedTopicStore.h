#pragma once
#include "Definitions.h"
#include "Utils.h"
#include <string>

namespace me
{

// TValType, TValTypeConstructor should be pointer types.
// TPathType needs .Levels(), PeekLevel( iCurLevel + 1 ) return me::pcview
// TValTypeConstructor ->Create(TPathType).
template<typename TValType, typename TPathType, typename TValTypeConstructor>
class Node
{
public:
   Node( size_t aiDepth, TValTypeConstructor apAlloc );
   ~Node();

   TValType AddNode( TPathType apPathType );
   TValType FindNode( TPathType apPathType );
   TValType DeleteNode( TPathType apPathType );

   std::vector<TValType> FindMatches( TPathType );

private:
   size_t m_iDepth;
   TValType m_pValue;
   TValTypeConstructor m_pValTypeConstructor;
   
   typedef 
      std::map<
      me::utils::pcview, 
      std::shared_ptr<Node<TValType, TPathType, TValTypeConstructor>>, 
      me::utils::pcviewless
      > 
      NodeMap;
   NodeMap m_mapChildren;

};

template<typename TValType, typename TPathType, typename TValTypeConstructor>
inline Node<TValType, TPathType, TValTypeConstructor>::Node( size_t aiDepth, TValTypeConstructor apAlloc )
   : m_iDepth(aiDepth), m_pValTypeConstructor(apAlloc)
{
}

template<typename TValType, typename TPathType, typename TValTypeConstructor>
inline Node<TValType, TPathType, TValTypeConstructor>::~Node()
{
}

template<typename TValType, typename TPathType, typename TValTypeConstructor>
inline TValType Node<TValType, TPathType, TValTypeConstructor>::AddNode( TPathType apPathType )
{
   std::vector<TValType> m_vecSubs;
   auto mapCurLevel = &m_mapChildren;
   std::stack<Node<TValType, TPathType, TValTypeConstructor>*> stk;

   stk.push( this );

   size_t iDepth = m_iDepth;
   size_t iTargetDepth = apPathType.Levels();
   while( !stk.empty() )
   {
      auto pNode = stk.top();
      stk.pop();

      if( iTargetDepth == iDepth )
      {
         m_pValue = m_pValTypeConstructor->Create( apPathType );
         return m_pValue;
      }

      auto next = apPathType.PeekLevel( iDepth + 1 );
      mapCurLevel = &pNode->m_mapChildren;

      auto iter_exists = mapCurLevel->find( next );
      if( iter_exists != mapCurLevel->end() )
      {
         stk.push( &*iter_exists->second );
      }
      else
      {
         auto item = mapCurLevel->emplace(
            next,
            std::make_shared<Node<TValType, TPathType, TValTypeConstructor>>
            ( iDepth + 1, m_pValTypeConstructor )
         );
         stk.push( &*item.first->second );
      }
      iDepth++;
   }
}

template<typename TValType, typename TPathType, typename TValTypeConstructor>
inline TValType Node<TValType, TPathType, TValTypeConstructor>::FindNode( TPathType apPathType )
{
   std::vector<TValType> m_vecSubs;
   auto mapCurLevel = &m_mapChildren;
   std::stack<Node<TValType, TPathType, TValTypeConstructor>*> stk;

   stk.push( this );

   size_t iDepth = m_iDepth;
   size_t iTargetDepth = apPathType.Levels();
   while( !stk.empty() )
   {
      auto pNode = stk.top();
      stk.pop();

      if( iTargetDepth == iDepth )
      {
         return pNode;
      }

      auto next = apPathType.PeekLevel( iDepth + 1 );
      mapCurLevel = &pNode->m_mapChildren;

      auto iter_exists = mapCurLevel->find( next );
      if( iter_exists != mapCurLevel->end() )
      {
         stk.push( &*iter_exists->second );
      }
      iDepth++;
   }
}

template<typename TValType, typename TPathType, typename TValTypeConstructor>
inline TValType Node<TValType, TPathType, TValTypeConstructor>::DeleteNode( TPathType apPathType )
{
   TValType pRetval = nullptr;
   std::vector<TValType> m_vecSubs;
   auto mapCurLevel = &m_mapChildren;
   std::stack<Node<TValType, TPathType, TValTypeConstructor>*> stk;
   std::stack<NodeMap::iterator> stk_visited;

   stk.push( this );

   size_t iDepth = m_iDepth;
   size_t iTargetDepth = apPathType.Levels();
   while( !stk.empty() )
   {
      auto pNode = stk.top();
      stk.pop();
      
      auto next = apPathType.PeekLevel( iDepth + 1 );
      mapCurLevel = &pNode->m_mapChildren;

      auto iter_exists = mapCurLevel->find( next );
      if( iter_exists != mapCurLevel->end() )
      {
         if( iTargetDepth == iDepth + 1)
         {
            pRetval = iter_exists->second->m_pValue;
            mapCurLevel->erase(iter_exists);
         }
         else
         {
            stk_visited.push( iter_exists );
            stk.push( &*iter_exists->second );
         }
      }
      iDepth++;
   }

   while( stk_visited.size() > 1 )
   {
      auto pNodeIter = stk_visited.top();
      auto pNode = pNodeIter->second;
      stk.pop();

      auto pParentIter = stk_visited.top();
      auto pParent = pParentIter->second;

      if( pNode->m_mapChildren.size() == 0 && !pNode->m_pValue )
      {
         pParent->m_mapChildren.erase( pNodeIter );
      }
   }

   if( stk_visited.size() == 1 )
   {
      auto pNodeIter = stk_visited.top();
      auto pNode = pNodeIter->second;
      stk_visited.pop();

      if( pNode->m_mapChildren.size() == 0 && !pNode->m_pValue )
      {
         this->m_mapChildren.erase( pNodeIter );
      }
   }

   return pRetval;
}

template<typename TValType, typename TPathType, typename TValTypeConstructor>
inline std::vector<TValType> Node<TValType, TPathType, TValTypeConstructor>::FindMatches( TPathType )
{
   return std::vector<TValType>();
}

}




