#pragma once
#include "stdafx.h"
#include <exception>

class MalformedPacket : public std::exception
{
public:
   MalformedPacket() {};
   ~MalformedPacket() {};
};

