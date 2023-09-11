//
// Created by Harry on 8/4/2023.
//

#ifndef XLUA_MEMORYALLOCATOR_H
#define XLUA_MEMORYALLOCATOR_H
#include "../LXX.h"

namespace LXX
{


class MemoryAllocator
{
public:
    static MemoryAllocator& GetInstance();
    void* Allocate( u64 size );

    template<typename T>
    T* Allocate()
    {
        return new( Allocate(sizeof( T ) ) ) T() ;
    }

    void Free(void* ptr);
    void* Realloc(void* ptr, u64 size);
private:
    static MemoryAllocator* m_instance;
private:
    MemoryAllocator() = default;
    MemoryAllocator(const MemoryAllocator&) = delete;
};  // class MemoryAllocator


} // namespace LXX

#endif //XLUA_MEMORYALLOCATOR_H
