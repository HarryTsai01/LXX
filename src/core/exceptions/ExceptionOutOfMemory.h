//
// Created by Harry on 8/5/2023.
//

#ifndef XLUA_EXCEPTIONOUTOFMEMORY_H
#define XLUA_EXCEPTIONOUTOFMEMORY_H
#include <core/LXX.h>
#include "ExceptionBase.h"


namespace LXX
{

class ExceptionOutOfMemory : public ExceptionBase
{
public:
    ExceptionOutOfMemory( u64 size );

    virtual String* ToString() const override;
private:
    u64 size;
}; // ExceptionOutOfMemory


} // LXX




#endif //XLUA_EXCEPTIONOUTOFMEMORY_H
