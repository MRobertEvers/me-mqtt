#pragma once
#include <exception>

class MalformedFixedHeader : public std::exception
{
public:
   MalformedFixedHeader() {};
   ~MalformedFixedHeader() {};
};

