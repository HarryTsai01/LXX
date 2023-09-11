//
// Created by Harry on 8/5/2023.
//

#ifndef XLUA_EXCEPTIONINVALIDPARAMETER_H
#define XLUA_EXCEPTIONINVALIDPARAMETER_H
#include "ExceptionBase.h"

namespace LXX
{


class ExceptionInvalidParameter : public ExceptionBase
{
public:
    ExceptionInvalidParameter( s32 idx , const char *message );

    virtual String* ToString() const {
        return String::Format("Idx:%d Message:%s\r\n",
                              Index,
                              Message->GetData()
                              );
    }

private:
    s32 Index;
    String *Message;
}; // ExceptionInvalidParameter

} // LXX

#endif //XLUA_EXCEPTIONINVALIDPARAMETER_H
