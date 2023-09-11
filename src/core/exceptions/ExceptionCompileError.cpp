//
// Created by Harry on 8/10/2023.
//

#include "ExceptionCompileError.h"
#include <core/objects/String/StringTable.h>

namespace LXX
{

ExceptionCompileError::ExceptionCompileError( String * msg,const String* fileName, s32 lineNo )
{
    this->msg = msg;
    this->fileName = fileName;
    this->lineNo = lineNo;
}

ExceptionCompileError::ExceptionCompileError( const char *msg , const String *fileName , s32 lineNo )
    : ExceptionCompileError( NEW_STRING( msg ) , fileName , lineNo )
{
}


String* ExceptionCompileError::ToString() const
{
    const char* szFileName = fileName ? fileName->GetData() : "";
    return String::Format( " Compile Error: \n fileName:%s \n lineNo:%d \n detail:%s \n",
                           szFileName ,
                           lineNo ,
                           msg->GetData()
                           );
}



} // LXX