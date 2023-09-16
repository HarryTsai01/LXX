//
// Created by Harry on 9/16/2023.
//

#ifndef XLUA_MEMORYACHIVEWRITER_H
#define XLUA_MEMORYACHIVEWRITER_H
#include "MemoryArchive.h"

namespace LXX
{


class MemoryArchiveWriter : public MemoryArchive
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    virtual u32 Serialize( void* buffer, u32 size ) override ;
    virtual bool IsReader() const { return false; }

};


} // LXX

#endif //XLUA_MEMORYACHIVEWRITER_H
