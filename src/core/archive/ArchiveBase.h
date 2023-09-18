//
// Created by Harry on 9/15/2023.
//

#ifndef XLUA_ARCHIVEBASE_H
#define XLUA_ARCHIVEBASE_H
#include <core/objects/GCObject.h>

namespace LXX
{

class ArchiveBase
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    enum class SeekMode
    {
        Begin,
        Current,
        End
    };

    ArchiveBase() = default;
    virtual ~ArchiveBase() = default;

    virtual u32 Serialize( void* buffer, u32 size ) = 0;
    virtual u32 Serialize( const void* buffer, u32 size ) = 0;
    virtual bool IsReader() const = 0;
    bool IsWriter() const { return !IsReader(); }

    virtual u64 GetFileSize() = 0;
    virtual u64 Tell() = 0;
    virtual u64 Seek( u64 offset , SeekMode mode ) = 0;

};

} // LXX

#endif //XLUA_ARCHIVEBASE_H
