//
// Created by Harry on 8/10/2023.
//

#ifndef XLUA_EXCEPTIONCOMPILEERROR_H
#define XLUA_EXCEPTIONCOMPILEERROR_H
#include <string>
#include <sstream>
#include <core/LXX.h>
#include <core/exceptions/ExceptionBase.h>
#include <core/objects/String/String.h>

namespace LXX
{

using namespace std;

class ExceptionCompileError : public ExceptionBase
{
public:
    ExceptionCompileError( const char* msg,const String* fileName, s32 lineNo );
    ExceptionCompileError( String * msg,const String* fileName, s32 lineNo );

    virtual String* ToString() const override;
private:
    const String *fileName;
    const String *msg;
    s32 lineNo;
};

} // LXX

#endif //XLUA_EXCEPTIONCOMPILEERROR_H
