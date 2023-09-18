//
// Created by Harry on 9/14/2023.
//

#ifndef XLUA_UTILITIES_H
#define XLUA_UTILITIES_H

namespace LXX
{


template<typename T>
T Max(T a, T b)
{
    return a > b ? a : b;
}


template<typename T>
T Min(T a, T b)
{
    return a < b ? a : b;
}


template<typename T>
T Clamp(T value, T min, T max )
{
    return Min(Max(value, min), max);
}

template<typename T , bool bigEndian = true>
void ReadFromBuffer( T& dest , u8* buffer )
{
    u32 byteNum = sizeof( T );
    u8 *destPtr = reinterpret_cast<u8*>( &dest );
    for( u32 i = 0 ; i < byteNum ; i++ )
        destPtr[ bigEndian ? i : byteNum - i - 1 ] = buffer[i];
}

template<typename T , bool bigEndian = true>
void WriteToBuffer( const T& data , u8* buffer )
{
    u32 byteNum = sizeof( T );
    const u8 * src = reinterpret_cast<const u8*>( &data );
    for( u32 i = 0 ; i < byteNum ; i++ )
        buffer[i] = src[ bigEndian ? i :byteNum - i - 1 ];
}

}

#endif //XLUA_UTILITIES_H
