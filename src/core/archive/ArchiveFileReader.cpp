//
// Created by Harry on 9/15/2023.
//

#include "ArchiveFileReader.h"

namespace LXX
{


bool ArchiveFileReader::Open()
{
    _file = fopen( _fileName->GetData() , _isBinary ? "rb" : "r" );

    return _file != nullptr;
}


u32 ArchiveFileReader::Serialize( void* buffer, size_t size )
{
    if( !_file )
        return 0;

    return fread( buffer, size, 1, _file );
}


} // LXX