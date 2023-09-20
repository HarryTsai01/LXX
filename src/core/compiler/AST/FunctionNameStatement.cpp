//
// Created by Harry on 8/30/2023.
//

#include "FunctionNameStatement.h"

namespace LXX
{


FunctionNameStatement::FunctionNameStatement( u32 lineNo )
 : StatementBase( lineNo )
 , _lastConcatToken( '.' )
{

}


void FunctionNameStatement::SetLastConcatToken( s32 lastConcatToken )
{
    _lastConcatToken = lastConcatToken;
}


void FunctionNameStatement::AddName( StatementBase* name )
{
    _names.PushBack( name );
}


} // xLua