//
// Created by Harry on 9/16/2023.
//

#include "MemoryArchive.h"
#include <core/Utilities.h>

namespace LXX
{


MemoryArchive::MemoryArchive()
 : _pos( 0 )
{

}


u64 MemoryArchive::GetFileSize()
{
    return _data.Size();
}


u64 MemoryArchive::Tell()
{
    return _pos;
}


u64 MemoryArchive::Seek( u64 offset , SeekMode mode )
{
    if( mode == SeekMode::Begin )
        _pos = offset;
    else if( mode == SeekMode::Current )
        _pos += offset;
    else if( mode == SeekMode::End )
        _pos = _data.Size() - offset;

    _pos = Clamp<u64>( _pos , 0 , _data.Size() );

    return _pos;
}


} // LXX