//
// Created by Harry on 8/15/2023.
//

#include "FunctionCallExpression.h"
#include <core/objects/GCObjectCollector.h>

namespace LXX
{


FunctionCallExpression::FunctionCallExpression( u32 lineNo )
    : SimpleExpression( lineNo )
    , _functionName( nullptr )
    , _varExpression( nullptr )
    , _arguments( nullptr )
{

}


FunctionCallExpression::~FunctionCallExpression()
{
}


void FunctionCallExpression::CollectReferences( GCObjectCollector& collector )
{
    collector.AddObject( _functionName );
    collector.AddObject( _varExpression );
    collector.AddObject( _arguments );
}

} // LXX