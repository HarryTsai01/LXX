//
// Created by Harry on 8/4/2023.
//

#ifndef XLUA_XLUAVALUE_H
#define XLUA_XLUAVALUE_H
#include "LXX.h"
#include <cassert>
#include <core/objects/LuaClosure.h>
#include <core/objects/string/String.h>

namespace LXX
{

class Table;
class Value
{
public:
    Value();
    Value( s32 val );
    Value( f64 val );
    Value( const char * val );
    Value( String * val );
    Value( Table * val );
    Value( CFunction val , bool isLightCFunction = true );
    Value( LuaClosure * val );

    bool operator == ( const Value &other ) const;

    u32 GetHashValue() const;


    template<typename SubType>
    static u32 MakeValueType(  ValueType type, SubType subType )
    {
        u32 subTypeFlag = ( (u32)subType & 0xf ) << 4 ;
        u32 typeFlag = (u32)type & 0xf;
        return subTypeFlag | typeFlag ;
    }

    template<typename SubType>
    void SetType( ValueType type, SubType subType )
    {
        flags = (flags & 0xffffff00 ) | MakeValueType( type, subType );
    }

    template<typename SubType>
    void GetType( ValueType &type, SubType &subType ) const
    {
        type = GetType();
        subType = GetSubType<SubType>();
    }

    ValueType GetType() const
    {
        return ValueType( flags & 0xf );
    }

    template<typename SubType>
    void GetSubType( SubType &subType ) const
    {
        subType = SubType( (flags >> 4) & 0xf );
    }


    bool IsNumber() const
    {
        return ValueType( flags & 0xf ) == ValueType::Number;
    }


    bool IsReal() const
    {
        return IsNumber() && NumberType( ( flags >> 4 ) & 0xf ) == NumberType::Real ;
    }


    bool IsInteger() const
    {
        return IsNumber() && NumberType( ( flags >> 4 ) & 0xf ) == NumberType::Integer ;
    }


    bool IsLightUserData() const
    {
        return ValueType( flags & 0xf ) == ValueType::LightUserdata;
    }


    bool  IsBoolean() const
    {
        return ValueType( flags & 0xf ) == ValueType::Boolean;
    }


    bool IsString() const
    {
        return ValueType( flags & 0xf ) == ValueType::String;
    }


    bool IsShortString() const
    {
        return IsString() && StringType( ( flags >> 4 ) & 0xf ) == StringType::Short;
    }


    bool IsLongString() const
    {
        return IsString() && StringType( ( flags >> 4 ) & 0xf ) == StringType::Long;
    }


    bool IsNil() const
    {
        return ValueType( flags & 0xf ) == ValueType::Nil;
    }


    bool IsTable() const
    {
        return ValueType( flags & 0xf ) == ValueType::Table;
    }

    bool IsFunction() const
    {
        return ValueType( flags & 0xf ) == ValueType::Function;
    }


    bool IsLuaClosure() const
    {
        return IsFunction() && FunctionType( ( flags >> 4 ) & 0xf ) == FunctionType::LuaClosure;
    }


    bool IsLightCFunction() const
    {
        return IsFunction() && FunctionType( ( flags >> 4 ) & 0xf ) == FunctionType::LightCFunction;
    }


    bool IsCClosure() const
    {
        return IsFunction() && FunctionType( ( flags >> 4 ) & 0xf ) == FunctionType::CClosure;
    }


    bool IsThread() const
    {
        return ValueType( flags & 0xf ) == ValueType::Thread;
    }

    template<typename T>
    T As() const
    {
        return static_cast<T>( AsInt() );
    }


    s32 AsInt() const
    {
        assert( IsNumber() );
        return IsInteger() ? i : (s32)r;
    }


    f64 AsReal() const
    {
        assert( IsNumber() );
        return IsReal() ? r : i;
    }

    bool AsBoolean() const
    {
        assert( IsBoolean() );
        return b;
    }

    String* AsString() const
    {
        assert( IsString() );
        return s;
    }

    CFunction AsLightCFunction() const
    {
        assert( IsLightCFunction() );
        return f;
    }


    CFunction AsCClosure() const
    {
        assert( IsCClosure() );
        return f;
    }


    LuaClosure* AsLuaClosure() const
    {
        assert( IsLuaClosure() );
        return lc;
    }


    void* AsLightUserData() const
    {
        assert( IsLightUserData() );
        return p;
    }

    Table* AsTable() const
    {
        assert( IsTable() );
        return t;
    }

    void Set( u32 value )
    {
        SetAsInt( value );
    }
    void Set( s32 value )
    {
        SetAsInt( value );
    }
    void Set( f64 value )
    {
        SetAsReal( value );
    }
    void Set( bool value )
    {
        SetAsBoolean( value );
    }
    void Set( String *value )
    {
        SetAsString( value );
    }
    void Set( Table *value )
    {
        SetAsTable( value );
    }
    void Set( LuaClosure *value )
    {
        SetAsLuaClosure( value );
    }
    void Set( Value* value )
    {
        if( value == nullptr )
            SetAsNil();
        else
            *this = *value;
    }


    void SetAsInt( s32 value )
    {
        SetType( ValueType::Number , NumberType::Integer );
        i = value;
    }


    void SetAsReal( f64 value )
    {
        SetType( ValueType::Number , NumberType::Real );
        r = value;
    }


    void SetAsLightUserData( void *value )
    {
        SetType(ValueType::LightUserdata , 0 );
        p = value;
    }


    void SetAsBoolean( bool value )
    {
        SetType( ValueType::Boolean , 0 );
        b = value;
    }


    void SetAsNil()
    {
        SetType( ValueType::Nil , 0 );
        p = nullptr;
    }

    void SetAsString( String *value )
    {
        SetType( ValueType::String , value->GetType() );
        s = value;
    }

    void SetAsTable( Table *value )
    {
        SetType( ValueType::Table , 0 );
        t = value;
    }


    void SetAsCClosure( CFunction value )
    {
        SetType( ValueType::Function , FunctionType::CClosure );
        f = value;
    }


    void SetAsLightCFunction( CFunction value )
    {
        SetType( ValueType::Function , FunctionType::LightCFunction );
        f = value;
    }


    void SetAsLuaClosure( LuaClosure *value )
    {
        SetType( ValueType::Function , FunctionType::LuaClosure );
        lc = value;
    }

    String* ToString();

private:
    union
    {
        void *p;
        bool b;
        s32 i;
        f64 r;
        CFunction f;
        String *s;
        LuaClosure *lc;
        Table *t;
    };
    u32 flags;
};

template<>
u32 Hash( Value val );
template<>
s32 Value::As() const;
template<>
f64 Value::As() const;
template<>
bool Value::As() const;
template<>
String *Value::As() const;
template<>
Table *Value::As() const;
template<>
LuaClosure *Value::As() const;

}

#endif //XLUA_XLUAVALUE_H
