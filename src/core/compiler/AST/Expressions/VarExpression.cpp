//
// Created by Harry on 8/29/2023.
//

#include "VarExpression.h"

namespace LXX
{

VarExpression::VarExpression( StatementBase* statement )
{
    _statements.PushBack( statement )   ;
}


} // xLua