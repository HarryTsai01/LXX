//
// Created by Harry on 8/9/2023.
//

#include "StringUtil.h"
#include "core/mem/MemoryAllocator.h"

namespace LXX
{

template<>
u32 Hash( const char* key )
{
    //return StringUtil::Hash( key , std::strlen( key ) );
    const int p = 31;
    const int m = 1e9 + 9;
    long long hash_value = 0;
    long long p_pow = 1;
    while( key[0] != '\0' )
    {
        hash_value = (hash_value + ( key[0] - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;

        ++ key;
    }
    return hash_value;
}


template<>
bool IsEqual( const char* lhs , const char* rhs )
{
    return std::strcmp( lhs , rhs ) == 0;
}


u32 StringUtil::Hash(const char *str, LXX::u32 len)
{
    u32 h = len;
    u32 step = len >> 5;
    u32 l1;
    for (l1 = len; l1 >= step; l1 -= step)
        h = h ^ ((h << 5) + (h >> 2) + (str[l1 - 1]));
    return h;
}


String* StringUtil::CreateLongString( u32 len )
{
    String *newString = new ( len ) String(len );
    return newString;
}

String* StringUtil::NewString( const char* begin , const char * end )
{
    if( end <= begin ) return nullptr;
    u32 len = end - begin;
    String* newStr = CreateLongString( len );
    Strncpy( newStr->GetData() , begin , len );
    newStr->GetData()[len] = '\0';
    return newStr;
}


void StringUtil::Strncpy(char *dest, const char *src, u32 len)
{
    std::strncpy( dest , src , len );
}

}
