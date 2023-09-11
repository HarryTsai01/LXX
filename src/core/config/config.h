//
// Created by Harry on 8/4/2023.
//

#ifndef XLUA_CONFIG_H
#define XLUA_CONFIG_H
#include "../LXX.h"

namespace LXX
{

class Config
{

public:
    static constexpr u32 STACK_MIN = 20 ;

    static constexpr u32 STACK_SIZE = 40 ;

    static constexpr u32 EXTRA_STACK_SIZE = 40 ;


    static constexpr f32 GC_INTERVAL = 10.0f ;

    /*
     *  during mark phase, when the mark count reach this value, would stop and continue next tick
     */
    static constexpr u32 GC_MARK_LIMIT = 10000 ;

    static constexpr u32 GC_SWEEP_LIMIT = 1000 ;


    /* String table min slot size */
    static constexpr u32 STRING_TABLE_MIN_SLOT_SIZE = 64 ;

    /* max short string length */
    static constexpr u32 MAX_SHORT_STRING_LENGTH = 40 ;
};

}

#endif //XLUA_CONFIG_H
