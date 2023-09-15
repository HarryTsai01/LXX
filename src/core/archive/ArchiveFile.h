//
// Created by Harry on 9/15/2023.
//

#ifndef XLUA_ARCHIVEFILE_H
#define XLUA_ARCHIVEFILE_H
#include "ArchiveBase.h"
#include <core/objects/string/String.h>
#include <cstdio>

namespace LXX
{


class ArchiveFile : public ArchiveBase
{
public:
    enum class SeekMode
    {
        Begin,
        Current,
        End
    };
    ArchiveFile( String *fileName , bool isBinary );
    ArchiveFile( const char *fileName , bool isBinary )
    : ArchiveFile( NEW_STRING( fileName ) ,  isBinary )
    {

    }

    virtual ~ArchiveFile();

    void Close();

    u64 GetFileSize();
    u64 Tell();
    u64 Seek( u64 offset , SeekMode mode );
protected:
    virtual bool Open() = 0;
private:
    ArchiveFile( const ArchiveFile &other ) = delete;
    ArchiveFile( ArchiveFile &&other ) = delete;
    ArchiveFile &operator=( const ArchiveFile &other ) = delete;
protected:
    String *_fileName;
    FILE *_file;
    bool _isBinary;
};


} // LXX

#endif //XLUA_ARCHIVEFILE_H
