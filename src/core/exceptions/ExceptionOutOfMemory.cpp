//
// Created by Harry on 8/5/2023.
//

#include "ExceptionOutOfMemory.h"

namespace LXX
{

ExceptionOutOfMemory::ExceptionOutOfMemory(LXX::u64 size)
{
    this->size = size;
}


String* ExceptionOutOfMemory::ToString() const
{
    return String::Format( "Out of memory: %llu bytes", size);
}

}