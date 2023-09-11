//
// Created by Harry on 8/5/2023.
//

#include "ExceptionInvalidType.h"

namespace LXX
{


ExceptionInvalidType::ExceptionInvalidType( s32 stackIdx )
{
    this->stackIdx = stackIdx;
}


String* ExceptionInvalidType::ToString() const
{
    return String::Format( "Invalid type at stack index : %d" , stackIdx );
}

} // LXX