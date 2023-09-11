//
// Created by Harry on 8/5/2023.
//

#ifndef XLUA_EXCEPTIONINVALIDTYPE_H
#define XLUA_EXCEPTIONINVALIDTYPE_H
#include "ExceptionBase.h"

namespace LXX
{

class ExceptionInvalidType : public ExceptionBase
{
public:
    ExceptionInvalidType(s32 index );

    virtual String* ToString() const override;
private:
    s32 stackIdx;
};

} // LXX

#endif //XLUA_EXCEPTIONINVALIDTYPE_H
