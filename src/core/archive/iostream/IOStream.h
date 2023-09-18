//
// Created by Harry on 9/16/2023.
//

#ifndef XLUA_IOSTREAM_H
#define XLUA_IOSTREAM_H
#include "IOStreamBase.h"
#include <core/utilities.h>

namespace LXX
{

template< bool IsBigEndian = true >
class IOStream :  public IOStreamBase
{

public:
    using IOStreamBase::IOStreamBase;

    template<typename T>
    void SerializeData( const T& data )
    {
        u8 buffer[sizeof(T)];
        WriteToBuffer< T , IsBigEndian >( data , buffer );
        IOStreamBase::Serialize( static_cast<const void*>(buffer), 8 );
    }

    template<typename T>
    void SerializeData( T& data )
    {
        u8 buffer[sizeof(T)];
        IOStreamBase::Serialize( buffer, 8 );
        ReadFromBuffer<T , IsBigEndian >( data , buffer );
    }

    virtual void Serialize( u64& data )
    {
        SerializeData( data );
    }
    virtual void Serialize( s64& data )
    {
        SerializeData( data );
    }
    virtual void Serialize( f32& data )
    {
        SerializeData( data );
    }
    virtual void Serialize( f64& data )
    {
        SerializeData( data );
    }
    virtual void Serialize( bool& data )
    {
        IOStreamBase::Serialize( &data , 1 );
    }
    virtual void Serialize( u32& data )
    {
        SerializeData( data );
    }
    virtual void Serialize( s32& data )
    {
        SerializeData( data );
    }
    virtual void Serialize( u16& data )
    {
        SerializeData( data );
    }
    virtual void Serialize( s16& data )
    {
        SerializeData( data );
    }
    virtual void Serialize( u8& data )
    {
        IOStreamBase::Serialize( &data , 1 );
    }
    virtual void Serialize( s8& data )
    {
        IOStreamBase::Serialize( &data , 1 );
    }

    virtual void Serialize( const u64& data )
    {
        SerializeData( data );
    }
    virtual void Serialize( const s64& data )
    {
        SerializeData( data );
    }
    virtual void Serialize( const f32& data )
    {
        SerializeData( data );
    }
    virtual void Serialize( const f64& data )
    {
        SerializeData( data );
    }
    virtual void Serialize( const bool& data )
    {
        IOStreamBase::Serialize( &data , 1 );
    }
    virtual void Serialize( const u32& data )
    {
        SerializeData( data );
    }
    virtual void Serialize( const s32& data )
    {
        SerializeData( data );
    }
    virtual void Serialize( const u16& data )
    {
        SerializeData( data );
    }
    virtual void Serialize( const s16& data )
    {
        SerializeData( data );
    }
    virtual void Serialize( const u8& data )
    {
        IOStreamBase::Serialize( &data , 1 );
    }
    virtual void Serialize( const s8& data )
    {
        IOStreamBase::Serialize( &data , 1 );
    }
};

using IOStreamLE = IOStream< false >;
using IOStreamBE = IOStream< true >;


} // LXX

#endif //XLUA_IOSTREAM_H
