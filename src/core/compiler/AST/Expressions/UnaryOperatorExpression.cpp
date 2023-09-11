//
// Created by Harry on 8/28/2023.
//

#include "UnaryOperatorExpression.h"

namespace LXX
{


UnaryOperatorExpression::UnaryOperatorExpression( s32 tokenType , StatementBase* expression )
: _tokenType( tokenType )
, _expression( expression )
{
}


} // xLua