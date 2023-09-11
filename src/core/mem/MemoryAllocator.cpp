//
// Created by Harry on 8/4/2023.
//
#include "MemoryAllocator.h"
#include <cstdlib>
#include <core/exceptions/ExceptionOutOfMemory.h>

namespace LXX
{

MemoryAllocator*  MemoryAllocator::m_instance = nullptr;

MemoryAllocator& MemoryAllocator::GetInstance()
{
    if( m_instance == nullptr ) m_instance =  new MemoryAllocator();

    return *m_instance;
}


void* MemoryAllocator::Allocate(u64 size)
{
    auto ptr = malloc( size );

    if( ptr == nullptr ) throw ExceptionOutOfMemory( size );

    return ptr;
}

void MemoryAllocator::Free(void *ptr)
{
    free( ptr );
}


void* MemoryAllocator::Realloc(void *ptr, u64 size)
{
    auto newPtr = realloc( ptr, size );
    if( newPtr == nullptr ) throw ExceptionOutOfMemory( size );
    return newPtr;
}



}
