#pragma once
#include "AsioConnection.h"
#include <set>
#include <memory>

class AsioConnectionManager
{
public:
   AsioConnectionManager();
   ~AsioConnectionManager();

   void AddConnection( std::shared_ptr<AsioConnection> apConnection );
   void CloseConnection( std::shared_ptr<AsioConnection> apConnection );

private:
   std::set<std::shared_ptr<AsioConnection>> m_setConnections;
};
