#pragma once
#include "Definitions.h"
#include "Utils.h"
#include <string>
#include <algorithm>

namespace me
{

// TValType, TValTypeConstructor should be pointer types.
// TPathType needs .Levels(), PeekLevel( size_t ) return me::pcview
// TValTypeConstructor ->Create(TPathType).
// TDepthType is default constructable
template<
   typename TValType, typename TPathType,
   typename TValTypeConstructor,
   typename TDepthType=me::utils::pcview>
class Node
{
private:
   typedef Node<TValType, TPathType, 
      TValTypeConstructor, TDepthType> NodeType;
   typedef
      std::map<
      TDepthType,
      std::shared_ptr<NodeType>
   >
      NodeMapType;
   typedef std::pair<
      TDepthType,
      std::shared_ptr<NodeType>
   > NodeMapValType;

public:
   Node( size_t aiDepth, TValTypeConstructor apAlloc );
   ~Node();

   std::shared_ptr<NodeType> AddNode( TPathType apPathType );
   std::shared_ptr<NodeType> FindNode( TPathType apPathType );
   std::shared_ptr<NodeType> DeleteNode( TPathType apPathType );

   std::vector<std::shared_ptr<NodeType>> BFT(size_t aiMaxDepth=0xFFFFFFFF);
   std::shared_ptr<NodeType> FindDirectChild( TDepthType );
   size_t GetChildCount() const;

   size_t Depth() const;
   TValType Value();

private:
   size_t m_iDepth;
   TValType m_pValue;
   TValTypeConstructor m_pValTypeConstructor;
   NodeMapType m_mapChildren;

};

template<typename TValType, typename TPathType, typename TValTypeConstructor, typename TDepthType>
inline Node<TValType, TPathType, TValTypeConstructor, TDepthType>::
Node( size_t aiDepth, TValTypeConstructor apAlloc )
   : m_iDepth(aiDepth), m_pValTypeConstructor(apAlloc)
{
}

template<typename TValType, typename TPathType, typename TValTypeConstructor, typename TDepthType>
inline Node<TValType, TPathType, TValTypeConstructor, TDepthType>::
~Node()
{
}

template<typename TValType, typename TPathType, typename TValTypeConstructor, typename TDepthType>
inline std::shared_ptr<Node<TValType, TPathType, TValTypeConstructor, TDepthType>>
Node<TValType, TPathType, TValTypeConstructor, TDepthType>::
AddNode( TPathType apPathType )
{
   std::shared_ptr<NodeType> pRetval = nullptr;
   std::vector<TValType> m_vecSubs;
   auto mapCurLevel = &m_mapChildren;
   std::stack<NodeType*> stk;

   stk.push( this );

   size_t iDepth = m_iDepth;
   size_t iTargetDepth = apPathType.Levels();
   while( !stk.empty() )
   {
      auto pNode = stk.top();
      stk.pop();

      if( iTargetDepth == iDepth )
      {
         pNode->m_pValue = m_pValTypeConstructor->Create( apPathType );
         return pRetval;
      }

      auto next = apPathType.PeekLevel( iDepth + 1 );
      mapCurLevel = &pNode->m_mapChildren;

      auto iter_exists = mapCurLevel->find( next );
      if( iter_exists != mapCurLevel->end() )
      {
         pRetval = iter_exists->second;
         stk.push( &*iter_exists->second );
      }
      else
      {
         auto item = mapCurLevel->emplace(
            next,
            std::make_shared<NodeType>
            ( iDepth + 1, m_pValTypeConstructor )
         );
         pRetval = item.first->second;
         stk.push( &*item.first->second );
      }
      iDepth++;
   }

   return nullptr;
}

template<typename TValType, typename TPathType, typename TValTypeConstructor, typename TDepthType>
inline std::shared_ptr<Node<TValType, TPathType, TValTypeConstructor, TDepthType>>
Node<TValType, TPathType, TValTypeConstructor, TDepthType>::
FindNode( TPathType apPathType )
{
   std::shared_ptr<NodeType> pRetval = nullptr;
   std::vector<TValType> m_vecSubs;
   auto mapCurLevel = &m_mapChildren;
   std::stack<NodeType*> stk;

   stk.push( this );

   size_t iDepth = m_iDepth;
   size_t iTargetDepth = apPathType.Levels();
   while( !stk.empty() )
   {
      auto pNode = stk.top();
      stk.pop();

      if( iTargetDepth == iDepth )
      {
         return pRetval;
      }

      auto next = apPathType.PeekLevel( iDepth + 1 );
      mapCurLevel = &pNode->m_mapChildren;

      auto iter_exists = mapCurLevel->find( next );
      if( iter_exists != mapCurLevel->end() )
      {
         pRetval = iter_exists->second;
         stk.push( &*iter_exists->second );
      }
      iDepth++;
   }

   return nullptr;
}

template<typename TValType, typename TPathType, typename TValTypeConstructor, typename TDepthType>
inline std::shared_ptr<Node<TValType, TPathType, TValTypeConstructor, TDepthType>>
Node<TValType, TPathType, TValTypeConstructor, TDepthType>::
DeleteNode( TPathType apPathType )
{
   std::shared_ptr<NodeType> pRetval = nullptr;
   std::vector<TValType> m_vecSubs;
   auto mapCurLevel = &m_mapChildren;
   std::stack<NodeType*> stk;
   std::stack<NodeMapType::iterator> stk_visited;

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
         pRetval = iter_exists->second;
         if( iTargetDepth == iDepth + 1)
         {
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

template<typename TValType, typename TPathType, typename TValTypeConstructor, typename TDepthType>
inline std::vector<std::shared_ptr<Node<TValType, TPathType, TValTypeConstructor, TDepthType>>>
Node<TValType, TPathType, TValTypeConstructor, TDepthType>::BFT( size_t aiMaxDepth )
{
   std::vector<std::shared_ptr<NodeType>> vecNodes;
   auto pCurStk = new std::stack<std::shared_ptr<NodeType>>();
   auto pNextStk = new std::stack<std::shared_ptr<NodeType>>();
   NodeType* pNode;
   size_t iCurLevel = 0;

   if( aiMaxDepth == 0 )
   {
      return vecNodes;
   }
   else
   {
      pNode = this;
      for( auto iter = pNode->m_mapChildren.begin(); iter != pNode->m_mapChildren.end(); iter++ )
      {
         pNextStk->push( iter->second );
         vecNodes.push_back( iter->second );
      }
      iCurLevel++;
   }

   while( !pCurStk->empty() || !pNextStk->empty() && iCurLevel < aiMaxDepth )
   {
      if( pCurStk->empty() )
      {
         auto tmp = pCurStk;
         pCurStk = pNextStk;
         pNextStk = tmp;

         pNode = &*pCurStk->top();
         pCurStk->pop();

         iCurLevel++;
      }

      for( auto iter = pNode->m_mapChildren.begin(); 
         iter != pNode->m_mapChildren.end(); iter++ )
      {
         pNextStk->push( iter->second );
         vecNodes.push_back( iter->second );
      }
   }

   delete pCurStk;
   delete pNextStk;

   return vecNodes;
}


template<typename TValType, typename TPathType, typename TValTypeConstructor, typename TDepthType>
inline std::shared_ptr<Node<TValType, TPathType, TValTypeConstructor, TDepthType>>
Node<TValType, TPathType, TValTypeConstructor, TDepthType>::
FindDirectChild( TDepthType aid )
{
   auto iter_find = m_mapChildren.find( aid );
   if( iter_find != m_mapChildren.end() )
   {
      return iter_find->second;
   }
   else
   {
      return nullptr;
   }
}

template<typename TValType, typename TPathType, typename TValTypeConstructor, typename TDepthType>
inline size_t
Node<TValType, TPathType, TValTypeConstructor, TDepthType>::GetChildCount() const
{
   return m_mapChildren.size();
}

template<typename TValType, typename TPathType, typename TValTypeConstructor, typename TDepthType>
inline size_t Node<TValType, TPathType, TValTypeConstructor, TDepthType>::
Depth() const
{
   return m_iDepth;
}

template<typename TValType, typename TPathType, typename TValTypeConstructor, typename TDepthType>
inline TValType Node<TValType, TPathType, TValTypeConstructor, TDepthType>::
Value()
{
   return m_pValue;
}

}




