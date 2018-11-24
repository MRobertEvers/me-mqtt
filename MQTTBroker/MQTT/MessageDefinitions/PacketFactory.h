#pragma once
#include <memory>
#include <string>

class ControlPacket;

class PacketFactory
{
public:
   static ControlPacket* GetPacket( std::string const& aszData, unsigned char aiFixedHeaderSize );
};

