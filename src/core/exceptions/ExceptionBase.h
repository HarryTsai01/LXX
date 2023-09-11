//
// Created by Harry on 8/5/2023.
//

#ifndef XLUA_EXCEPTIONBASE_H
#define XLUA_EXCEPTIONBASE_H
#include <core/LXX.h>
#include <exception>
#include <core/objects/string/String.h>

using namespace std;

namespace LXX
{

class ExceptionBase : public exception
{
public:
    virtual String* ToString() const = 0;
}; // class ExceptionBase

} // namespace LXX



#endif //XLUA_EXCEPTIONBASE_H
