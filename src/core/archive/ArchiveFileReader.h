//
// Created by Harry on 9/15/2023.
//

#ifndef XLUA_ARCHIVEFILEREADER_H
#define XLUA_ARCHIVEFILEREADER_H
#include "ArchiveFile.h"

namespace LXX {

class ArchiveFileReader : public ArchiveFile
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    using ArchiveFile::ArchiveFile;

    virtual bool Open() ;
    virtual u32 Serialize( void* buffer, size_t size ) ;
    virtual bool IsReader() const { return true; }
};

} // LXX

#endif //XLUA_ARCHIVEFILEREADER_H
