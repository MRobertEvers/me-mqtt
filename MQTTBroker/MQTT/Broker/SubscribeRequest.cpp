#include "stdafx.h"
#include "SubscribeRequest.h"
namespace me
{
SubscribeRequest::SubscribeRequest( char const* apData, size_t aSize, unsigned char aQOS )
{
   Topic = std::make_shared<std::string>( apData, aSize );
   QOS = aQOS;
}
SubscribeRequest::SubscribeRequest( std::string aTopic, unsigned char aQOS )
{
   Topic = std::make_shared<std::string>( aTopic );
   QOS = aQOS;
}
}