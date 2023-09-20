//
// Created by Harry on 8/14/2023.
//

#include "IntegerNumberExpression.h"

namespace LXX
{


IntegerNumberExpression::IntegerNumberExpression( u32 lineNo ,  s32 value )
        : SimpleExpression( lineNo )
        , _value( value )
{
}


} // LXX