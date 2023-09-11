//
// Created by Harry on 8/5/2023.
//
#include "Stack.h"

namespace LXX
{


Value *Stack::GetValue(s32 idx)
{
    return IndexToValue( idx );
}

s32 Stack::GetInteger( s32 idx /* = -1 */, bool bCheck /* = false */)
{
    if( bCheck ) CheckIsInteger( idx );
    return IndexToValue( idx )->AsInt();
}


f64 Stack::GetReal( s32 idx /* = -1 */, bool bCheck /* = false */)
{
    if( bCheck ) CheckIsReal( idx );
    return IndexToValue( idx )->AsReal();
}


void* Stack::GetLightUserData( s32 idx, bool bCheck ) {
    if( bCheck ) CheckIsLightUserData( idx );
    return IndexToValue( idx )->AsLightUserData();
}


bool Stack::GetBoolean( s32 idx, bool bCheck )
{
    if( bCheck ) CheckIsBoolean( idx );
    return IndexToValue( idx )->AsBoolean();
}


String* Stack::GetString( s32 idx , bool bCheck )
{
    if( bCheck ) CheckIsString( idx );
    return IndexToValue( idx )->AsString();
}

CFunction Stack::GetLightCFunction( s32 idx, bool bCheck )
{
    if( bCheck ) CheckIsLightCFunction( idx );
    return IndexToValue( idx )->AsLightCFunction();
}


CFunction Stack::GetCClosure( s32 idx, bool bCheck )
{
    if( bCheck ) CheckIsCClosure( idx );
    return IndexToValue( idx )->AsCClosure();
}


LuaClosure* Stack::GetLuaClosure( s32 idx, bool bCheck)
{
    if( bCheck ) CheckIsLuaClosure( idx );
    return IndexToValue( idx )->AsLuaClosure();
}


template<>
s32 Stack::As<s32>( s32 idx , bool bCheck )
{
    return GetInteger( idx , bCheck );
}

template<>
f64 Stack::As<f64>( s32 idx , bool bCheck )
{
    return GetReal( idx , bCheck );
}


template<>
Value* Stack::As<Value*>( s32 idx , bool /*bCheck*/ )
{
    return GetValue( idx );
}

}

