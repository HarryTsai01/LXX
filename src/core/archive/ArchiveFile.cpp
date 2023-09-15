//
// Created by Harry on 9/15/2023.
//

#include "ArchiveFile.h"

namespace LXX
{


ArchiveFile::ArchiveFile( String *fileName , bool isBinary )
 : _fileName( fileName )
 , _file( nullptr )
 , _isBinary( isBinary )
{

}


ArchiveFile::~ArchiveFile()
{
    Close();
}


void ArchiveFile::Close()
{
    if( _file )
    {
        fclose( _file );
        _file = nullptr;
    }
}


u64 ArchiveFile::GetFileSize()
{
    if( !_file )
        return 0;
    u64 curPos = Tell();
    Seek( 0 , SeekMode::Begin );
    u64 size = Tell();
    Seek( curPos , SeekMode::Begin );
    return size;
}


u64 ArchiveFile::Tell()
{
    return ftell( _file );
}


u64 ArchiveFile::Seek( u64 offset , SeekMode mode )
{
    return fseek( _file , offset , static_cast<int>( mode ) );
}


} // LXX