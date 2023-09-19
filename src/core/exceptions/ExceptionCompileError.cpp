//
// Created by Harry on 8/10/2023.
//

#include "ExceptionCompileError.h"
#include <core/objects/String/StringTable.h>

namespace LXX
{

ExceptionCompileError::ExceptionCompileError( String * msg,const String* fileName, String* line,s32 lineNo )
    : _msg( msg )
    , _fileName( fileName )
    , _line( line )
    ,  _lineNo( lineNo )
{
}

ExceptionCompileError::ExceptionCompileError( const char *msg , const String *fileName , const char *line ,s32 lineNo )
    : ExceptionCompileError( NEW_STRING( msg ) , fileName , NEW_STRING( line ) ,lineNo )
{
}


String* ExceptionCompileError::ToString() const
{
    return String::Format(" %s:lineNo:%d %s detail:%s \n",
                          _fileName ? _fileName->GetData() : "" ,
                          _lineNo ,
                          _line ? _line->GetData() : "" ,
                          _msg ? _msg->GetData() : ""
                           );
}



} // LXX