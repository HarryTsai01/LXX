//
// Created by Harry on 9/16/2023.
//

#include "MemoryArchiveReader.h"
#include <core/utilities.h>
#include <cstring>

namespace LXX
{


MemoryArchiveReader::MemoryArchiveReader( const u8 * buffer , u32 size )
{
    _data.AddRange( buffer , size );
}

u32 MemoryArchiveReader::Serialize( void* buffer, u32 size )
{
    if( _pos >= _data.Size() ) return 0;

    u32 leftByteNum = _data.Size() - _pos;
    u32 byteNumToRead = Min< u32 > ( size , leftByteNum );

    std::memcpy( buffer , &_data[ _pos ] , byteNumToRead );

    _pos += byteNumToRead;

    return byteNumToRead;
}


} // LXX