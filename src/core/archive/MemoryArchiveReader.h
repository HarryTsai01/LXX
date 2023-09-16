//
// Created by Harry on 9/16/2023.
//

#ifndef XLUA_MEMORYACHIVEREADER_H
#define XLUA_MEMORYACHIVEREADER_H
#include "MemoryArchive.h"

namespace LXX
{

class MemoryArchiveReader : public MemoryArchive
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    using MemoryArchive::MemoryArchive;
    MemoryArchiveReader( const u8 * buffer, u32 size );
    virtual u32 Serialize( void* buffer, u32 size ) override;
    virtual bool IsReader() const { return true; }

};

} // LXX

#endif //XLUA_MEMORYACHIVEREADER_H
