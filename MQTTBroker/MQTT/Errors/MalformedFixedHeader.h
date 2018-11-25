#pragma once
#include <exception>
namespace me
{
class MalformedFixedHeader : public std::exception
{
public:
   MalformedFixedHeader() {};
   ~MalformedFixedHeader() {};
};

}