//
// Created by Harry on 9/16/2023.
//

#include "MemoryArchiveWriter.h"
#include <cstring>

namespace LXX
{


u32 MemoryArchiveWriter::Serialize( void* buffer, u32 size )
{
   if( _pos + size > _data.Size() )
       _data.AddZero( _pos + size - _data.Size() );

   std::memcpy( &_data[_pos] , buffer, size );

   return size;
}


} // LXX