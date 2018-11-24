#pragma once
#include <memory>
#include <string>

class ControlPacket;

static class PacketFactory
{
public:
   PacketFactory();
   ~PacketFactory();

   // static std::shared_ptr<ControlPacket> GetPacket(const char* apData, size_t aiDataSize );
   static std::shared_ptr<ControlPacket> GetPacket( std::string aszData, size_t aiFixedHeaderSize );
   
   // If you know the type you want.
   //template<typename T>
   //static std::shared_ptr<T> GetPacket();
};

