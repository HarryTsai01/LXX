//
// Created by Harry on 8/31/2023.
//

#ifndef XLUA_DISABLEENHANCEPOLICY_H
#define XLUA_DISABLEENHANCEPOLICY_H
#include <core/LXX.h>

namespace LXX
{


template<u32 InitialCapacity = 4>
class DisableEnhancePolicy
{
public:
    static u32 GetInitialCapacity() { return InitialCapacity; }
    static u32 EnhanceCapacity( u32 oldCapacity ) { return oldCapacity * 2; }
    static u32 ShrinkCapacity( u32 oldCapacity ) { return oldCapacity / 2; }
    static bool ShouldEnhance( u32 size, u32 capacity ) { return false; }
    static bool ShouldShrink( u32 size, u32 capacity ) { return false; }
    static u32 CalculateProperCapacity( u32 size ) { return size; }
};


}

#endif //XLUA_DISABLEENHANCEPOLICY_H
