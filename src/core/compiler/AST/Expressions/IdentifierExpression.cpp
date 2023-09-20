//
// Created by Harry on 8/16/2023.
//

#include "IdentifierExpression.h"
#include <core/objects/GCObjectCollector.h>

namespace LXX
{

IdentifierExpression::IdentifierExpression( u32 lineNo ,  String* identifier )
        :  SimpleExpression( lineNo )
        , _identifier( identifier )
{
}

void IdentifierExpression::CollectReferences( GCObjectCollector &collector )
{
    collector.AddObject( _identifier ) ;
}


} // LXX