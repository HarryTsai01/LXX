//
// Created by Harry on 9/15/2023.
//

#include "ArchiveFileWriter.h"

namespace LXX
{


bool ArchiveFileWriter::Open()
{
    _file =  fopen( _fileName->GetData(), _isBinary ? "wb" : "w" );
    return _file != nullptr;
}


u32 ArchiveFileWriter::Serialize( void* buffer, size_t size )
{
    if ( _file == nullptr )
        return 0;
    return fwrite( buffer, size, 1, _file );
}


} // LXX