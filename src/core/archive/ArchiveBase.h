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
public:
    ArchiveBase() = default;
    virtual ~ArchiveBase() = default;

    virtual void Serialize( void* buffer, size_t size ) = 0;
    virtual bool IsReader() const = 0;
    bool IsWriter() const { return !IsReader(); }

};

} // LXX

#endif //XLUA_ARCHIVEBASE_H
