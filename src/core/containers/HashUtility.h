//
// Created by Harry on 8/31/2023.
//

#ifndef XLUA_HASHUTILITY_H
#define XLUA_HASHUTILITY_H
#include <cstring>
#include <core/LXX.h>

namespace LXX
{
    template<typename Key>
    u32 Hash( Key key )
    {
        return std::hash<Key>()( key );
    }

    template<typename Key>
    bool IsEqual( Key lhs , Key rhs )
    {
        return lhs == rhs;
    }

    template<>
    u32 Hash( const char* key );

    template<>
    bool IsEqual( const char* lhs , const char* rhs );
};

#endif //XLUA_HASHUTILITY_H
