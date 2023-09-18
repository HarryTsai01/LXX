//
// Created by Harry on 9/16/2023.
//

#include "MemoryArchiveWriter.h"
#include <cstring>

namespace LXX
{


u32 MemoryArchiveWriter::Serialize( void* buffer, u32 size )
{
    assert(false);
    return 0;
}


u32 MemoryArchiveWriter::Serialize(const void *buffer, u32 size)
{
    if( _pos + size > _data.Size() )
        _data.AddZero( _pos + size - _data.Size() );

    std::memcpy( &_data[_pos] , buffer, size );

    _pos += size;

    return size;

}


} // LXX