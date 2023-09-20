//
// Created by Harry on 8/17/2023.
//

#include "AssignmentStatement.h"

namespace LXX
{

AssignmentStatement::AssignmentStatement( u32 lineNo)
    : StatementBase( lineNo )
    , _left( nullptr )
    , _right( nullptr )
{

}


AssignmentStatement::~AssignmentStatement()
{

}

void AssignmentStatement::SetOperators( StatementBase* left, StatementBase* right )
{
    this->_left = left;
    this->_right = right;
}

void AssignmentStatement::CollectReferences( GCObjectCollector& collector )
{
    collector.AddObject( _left );
    collector.AddObject( _right );
}

} // xLua