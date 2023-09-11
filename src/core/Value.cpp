//
// Created by Harry on 8/5/2023.
//
#include "Value.h"

namespace LXX
{

template<>
u32 Hash( Value val )
{
    return val.GetHashValue();
}

Value::Value()
{
    b = 0;
    flags = (u8)ValueType::Nil;
}


Value::Value( s32 val )
{
    SetAsInt( val );
}


Value::Value( f64 val )
{
    SetAsReal( val );
}


Value::Value( const char * val )
{
    if( val == nullptr ) SetAsNil();
    else SetAsString( NEW_STRING( val ) );
}


Value::Value( String * val )
{
    if( val == nullptr ) SetAsNil();
    else SetAsString( val );
}


Value::Value( Table * val )
{
    if( val == nullptr ) SetAsNil();
    else SetAsTable( val );
}


Value::Value( CFunction val , bool isLightCFunction )
{
    if( val == nullptr ) SetAsNil();
    else if( isLightCFunction ) SetAsLightCFunction( val );
    else SetAsCClosure( val );
}


Value::Value( LuaClosure * val )
{
    if( val == nullptr ) SetAsNil();
    else SetAsLuaClosure( val );
}


u32 Value::GetHashValue() const
{
    u32 hashValue = ( flags & 0xff ) << 24 ;
    ValueType valType = GetType();
    if( valType == ValueType::Number )
    {
        NumberType numberType;
        GetSubType( numberType );
        if( numberType == NumberType::Integer )
        {
            hashValue |= 0xffffff & ( u32 )( i );
        }
        else
        {
            hashValue |= 0xffffff & ( u32 )( r * ( 1 << 12 ) );
        }
    }
    else if( valType == ValueType::String )
    {
        hashValue |= 0xffffff & Hash( s );
    }
    else if( valType == ValueType::Table )
    {
        hashValue |= 0xffffff & Hash( t );
    }
    else if( valType == ValueType::Function )
    {
        FunctionType functionType;
        GetSubType( functionType );
        if( functionType == FunctionType::LuaClosure )
        {
            hashValue |= 0xffffff & Hash( lc );
        }
        else if( functionType == FunctionType::LightCFunction
            || functionType == FunctionType::CClosure )
        {
            hashValue |= 0xffffff & Hash( f );
        }
        else
        {
            assert( false );
        }
    }
    return hashValue;
}
template<>
s32 Value::As() const
{
    return AsInt();
}
template<>
f64 Value::As() const
{
    return AsReal();
}
template<>
bool Value::As() const
{
    return AsBoolean();
}
template<>
String *Value::As() const
{
    return AsString();
}
template<>
Table *Value::As() const
{
    return AsTable();
}
template<>
LuaClosure *Value::As() const
{
    return AsLuaClosure();
}

}
