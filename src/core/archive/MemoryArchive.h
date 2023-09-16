//
// Created by Harry on 9/16/2023.
//

#ifndef XLUA_MEMORYARCHIVE_H
#define XLUA_MEMORYARCHIVE_H
#include <core/archive/ArchiveBase.h>
#include <core/containers/Array.h>

namespace LXX
{

class MemoryArchive : public ArchiveBase
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    MemoryArchive();
    virtual ~MemoryArchive() = default;

    virtual u64 GetFileSize() override;
    virtual u64 Tell() override;
    virtual u64 Seek( u64 offset , SeekMode mode ) override;

    const u8 * GetBuffer() const { return &_data[0]; }
    u32 GetBufferSize() const { return _data.Size(); }
protected:
    Array<u8> _data;
    u64 _pos;
};

} // LXX

#endif //XLUA_MEMORYARCHIVE_H
