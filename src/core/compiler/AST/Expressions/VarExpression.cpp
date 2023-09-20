//
// Created by Harry on 8/29/2023.
//

#include "VarExpression.h"

namespace LXX
{

VarExpression::VarExpression( u32 lineNo ,  StatementBase* statement )
: ExpressionStatementBase( lineNo )
{
    _statements.PushBack( statement )   ;
}


} // xLua