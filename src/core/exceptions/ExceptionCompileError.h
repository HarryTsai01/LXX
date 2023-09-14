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
    ExceptionCompileError( String * msg,const String* fileName, String* line,s32 lineNo );
    ExceptionCompileError( const char* msg,const String* fileName, const char* line,s32 lineNo );

    virtual String* ToString() const override;
private:
    const String *_fileName;
    const String *_msg;
    const String* _line;
    s32 _lineNo;
};

} // LXX

#endif //XLUA_EXCEPTIONCOMPILEERROR_H
