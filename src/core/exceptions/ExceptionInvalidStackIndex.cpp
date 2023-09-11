//
// Created by Harry on 8/5/2023.
//

#include "ExceptionInvalidStackIndex.h"

namespace LXX
{


ExceptionInvalidStackIndex::ExceptionInvalidStackIndex( int stackIndex )
{
    this->stackIdx = stackIndex;
}


String* ExceptionInvalidStackIndex::ToString() const
{
    return String::Format( "Invalid stack index: %d", stackIdx );
}


} // LXX