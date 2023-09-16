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
    void SerializeData( T& data )
    {
        u8 buffer[sizeof(T)];
        if( _archive.IsReader() )
        {
            IOStreamBase::Serialize( buffer, 8 );
            ReadFromBuffer<T , IsBigEndian >( data , buffer );
        }
        else
        {
            WriteToBuffer< T , IsBigEndian >( data , buffer );
            IOStreamBase::Serialize( buffer, 8 );
        }
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
};

using IOStreamLE = IOStream< false >;
using IOStreamBE = IOStream< true >;


} // LXX

#endif //XLUA_IOSTREAM_H
