//
// Created by Harry on 8/28/2023.
//

#include "UnaryOperatorExpression.h"

namespace LXX
{


UnaryOperatorExpression::UnaryOperatorExpression( u32 lineNo , s32 tokenType , StatementBase* expression )
: ExpressionStatementBase( lineNo )
, _tokenType( tokenType )
, _expression( expression )
{
}


} // xLua