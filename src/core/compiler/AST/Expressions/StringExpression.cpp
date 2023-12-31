//
// Created by Harry on 8/14/2023.
//

#include "StringExpression.h"
#include <core/objects/GCObjectCollector.h>

namespace LXX
{


StringExpression::StringExpression( u32 lineNo , String* value )
        : SimpleExpression( lineNo )
        , _value( value )
{
}


StringExpression::~StringExpression()
{
}


void StringExpression::CollectReferences(GCObjectCollector &collector )
{
    collector.AddObject( _value );
}


} // LXX