//
// Created by Harry on 8/5/2023.
//

#ifndef XLUA_EXCEPTIONINVALIDSTACKINDEX_H
#define XLUA_EXCEPTIONINVALIDSTACKINDEX_H
#include "ExceptionBase.h"

namespace LXX
{

class ExceptionInvalidStackIndex : public ExceptionBase
{
public:
    ExceptionInvalidStackIndex( s32 stackIdx );

    virtual String* ToString() const override;

private:
    s32 stackIdx;
}; // ExceptionInvalidStackIndex

} // LXX
#endif //XLUA_EXCEPTIONINVALIDSTACKINDEX_H
