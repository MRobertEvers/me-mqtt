#include "stdafx.h"
#include "AsioConnectionManager.h"

AsioConnectionManager::AsioConnectionManager()
{
}

AsioConnectionManager::~AsioConnectionManager()
{
}

void
AsioConnectionManager::AddConnection( std::shared_ptr<AsioConnection> apConnection )
{
   m_setConnections.insert( apConnection );
   apConnection->Start();
   apConnection->Stop();
}

void 
AsioConnectionManager::CloseConnection( std::shared_ptr<AsioConnection> apConnection )
{
   m_setConnections.erase( apConnection );
   apConnection->ManagerClose();
}


