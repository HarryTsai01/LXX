//
// Created by Harry on 8/14/2023.
//

#include "BooleanExpression.h"

namespace LXX
{


BooleanExpression::BooleanExpression( u32 lineNo , bool value )
        : SimpleExpression( lineNo )
        , _value( value )
{

}


} // LXX