//
// Created by Harry on 8/15/2023.
//

#include "BinaryOperationExpression.h"
#include <core/objects/GCObjectCollector.h>

namespace LXX
{


BinaryOperationExpression::BinaryOperationExpression( u32 lineNo ,  s32 op , StatementBase* left, StatementBase* right )
    : ExpressionStatementBase( lineNo )
    , _operator( op )
    , _left( left )
    , _right( right )
{
}

BinaryOperationExpression::~BinaryOperationExpression()
{
}


void BinaryOperationExpression::CollectReferences( GCObjectCollector& collector )
{
    collector.AddObject( _left );
    collector.AddObject( _right );
}

} // LXX