//
// Created by Harry on 8/5/2023.
//
#include "Value.h"
#include <core/objects/Table.h>

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
 : Value()
{
    SetAsInt( val );
}


Value::Value( f64 val )
        : Value()
{
    SetAsReal( val );
}


Value::Value( const char * val )
        : Value()
{
    if( val == nullptr ) SetAsNil();
    else SetAsString( NEW_STRING( val ) );
}


Value::Value( String * val )
        : Value()
{
    if( val == nullptr ) SetAsNil();
    else SetAsString( val );
}


Value::Value( Table * val )
        : Value()
{
    if( val == nullptr ) SetAsNil();
    else SetAsTable( val );
}


Value::Value( CFunction val , bool isLightCFunction )
        : Value()
{
    if( val == nullptr ) SetAsNil();
    else if( isLightCFunction ) SetAsLightCFunction( val );
    else SetAsCClosure( val );
}


Value::Value( LuaClosure * val )
        : Value()
{
    if( val == nullptr ) SetAsNil();
    else SetAsLuaClosure( val );
}

bool Value::operator == ( const Value &other ) const
{
    if( flags != other.flags )
        return false;
    else
    {
        static UnorderedMap<u32, bool(*)(const Value&, const Value&)> equalFuncs
        {
                { MakeValueType( ValueType::Number , NumberType::Integer ) , []( const Value& a , const Value& b ) { return a.i == b.i; } },
                { MakeValueType( ValueType::Number , NumberType::Real ) , []( const Value& a , const Value& b ) { return a.r == b.r; } },
                { MakeValueType( ValueType::Boolean , 0) , []( const Value& a , const Value& b ) { return a.b == b.b; } },
                { MakeValueType( ValueType::String , StringType::Long ) , []( const Value& a , const Value& b ) { return a.s == b.s; } },
                { MakeValueType( ValueType::String , StringType::Short ) , []( const Value& a , const Value& b ) { return a.s == b.s; } },
                {  MakeValueType( ValueType::Nil , 0 ) , []( const Value& a , const Value& b ) { return true; } },
                { MakeValueType( ValueType::LightUserdata , 0 ) , []( const Value& a , const Value& b ) { return a.p == b.p; } },
                { MakeValueType( ValueType::Table , 0 ) , []( const Value& a , const Value& b ) { return a.t == b.t; } },
                { MakeValueType( ValueType::Function , FunctionType::LuaClosure ) , []( const Value& a , const Value& b ) { return a.lc == b.lc; } },
                { MakeValueType( ValueType::Function , FunctionType::LightCFunction ) , []( const Value& a , const Value& b ) { return a.f == b.f; } },
                { MakeValueType( ValueType::Function , FunctionType::CClosure ) , []( const Value& a , const Value& b ) { return a.f == b.f; } },
                { MakeValueType( ValueType::Thread , 0 ) , []( const Value& a , const Value& b ) { assert( false &&"not implement" ); return false; } },
                { MakeValueType( ValueType::FullUserdata , 0 ) , []( const Value& a , const Value& b ) { assert( false &&"not implement" ); return false; } },

        };

        return equalFuncs[ flags & 0xff ]( *this , other );
    }
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


String* Value::ToString()
{
    static UnorderedMap<u32, String*(*)(const Value&)> ToStringFuncs =
    {
        {  MakeValueType( ValueType::Number , NumberType::Integer ) , []( const Value& val ) { return String::Format( "%d" , val.i ); } },
        {  MakeValueType( ValueType::Number , NumberType::Real ) , []( const Value& val ) { return String::Format( "%f" , val.r ); } },
        {  MakeValueType( ValueType::Boolean , 0 ) , []( const Value& val ) { return NEW_STRING(val.b ? "true" : "false"); } },
        {  MakeValueType( ValueType::String , StringType::Long ) , []( const Value& val ) { return val.s; } },
        {  MakeValueType( ValueType::String , StringType::Short ) , []( const Value& val ) { return val.s; } },
        {  MakeValueType( ValueType::Nil , 0 ) , []( const Value& val ) { return NEW_STRING( "nil" ); } },
        {  MakeValueType( ValueType::LightUserdata , 0 ) , []( const Value& val ) { return String::Format( "lightUserdata(%p)" , val.p ); } },
        {  MakeValueType( ValueType::Table , 0 ) , []( const Value& val ) { return val.t->ToString(); } },
        {  MakeValueType( ValueType::Function , FunctionType::LuaClosure ) , []( const Value& val ) { return String::Format( "luaClosure(%p)" , val.lc ); } },
        {  MakeValueType( ValueType::Function , FunctionType::LightCFunction ) , []( const Value& val ) { return String::Format( "lightCFunction(%p)" , val.f ); } },
        {  MakeValueType( ValueType::Function , FunctionType::CClosure ) , []( const Value& val ) { return String::Format( "cClosure(%p)" , val.f ); } },
        {  MakeValueType( ValueType::Thread , 0 ) , []( const Value& val ) -> String*{ assert( false &&"not implement" ); return nullptr; } },
        {  MakeValueType( ValueType::FullUserdata , 0 ) , []( const Value& val )-> String*{ assert( false &&"not implement" ); return nullptr; } },
    };

    return ToStringFuncs[ flags & 0xff ]( *this );
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
