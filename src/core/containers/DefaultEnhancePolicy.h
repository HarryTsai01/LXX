//
// Created by Harry on 8/24/2023.
//

#ifndef XLUA_DEFAULTENHANCEPOLICY_H
#define XLUA_DEFAULTENHANCEPOLICY_H
#include <core/mem/MemoryAllocator.h>

namespace LXX
{


class DefaultEnhancePolicy
{
public:
    static u32 GetInitialCapacity() { return 4; }
    static u32 EnhanceCapacity( u32 oldCapacity ) { return oldCapacity * 2; }
    static u32 ShrinkCapacity( u32 oldCapacity ) { return oldCapacity / 2; }
    static bool ShouldEnhance( u32 size, u32 capacity ) { return size > capacity *2 / 3 ; }
    static bool ShouldShrink( u32 size, u32 capacity )
    {
        return size < capacity / 4
            && ShrinkCapacity( capacity ) > GetInitialCapacity() ;
    }
    static u32 CalculateProperCapacity( u32 size )
    {
        u32 newCapacity = DefaultEnhancePolicy::GetInitialCapacity();
        while ( DefaultEnhancePolicy::ShouldEnhance( size , newCapacity ) )
        {
            newCapacity = DefaultEnhancePolicy::EnhanceCapacity( newCapacity );
        }
        return newCapacity;
    }

};


}

#endif //XLUA_DEFAULTENHANCEPOLICY_H
