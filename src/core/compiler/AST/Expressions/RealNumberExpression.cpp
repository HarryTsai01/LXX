//
// Created by Harry on 8/14/2023.
//

#include "RealNumberExpression.h"

namespace LXX
{


RealNumberExpression::RealNumberExpression( u32 lineNo ,  f64 value )
        : SimpleExpression( lineNo )
        , _value( value )
{
}


} // LXX