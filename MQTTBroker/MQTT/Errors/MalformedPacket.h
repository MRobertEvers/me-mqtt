#pragma once
#include "stdafx.h"
#include <exception>
namespace me
{
class MalformedPacket : public std::exception
{
public:
   MalformedPacket() {};
   ~MalformedPacket() {};
};

}