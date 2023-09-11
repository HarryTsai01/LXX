//
// Created by Harry on 8/5/2023.
//
#include "Stack.h"

namespace LXX
{

bool Stack::IsNumber( s32 idx )
{
    CheckIndex( idx );
    return IndexToValue( idx )->IsNumber();
}


bool Stack::IsInteger( s32 idx )
{
    CheckIndex( idx );
    return IndexToValue( idx )->IsInteger();
}


bool Stack::IsReal( s32 idx )
{
    CheckIndex( idx );
    return IndexToValue( idx )->IsReal();
}


bool Stack::IsLightUserData( s32 idx )
{
    CheckIndex( idx );
    return IndexToValue( idx )->IsLightUserData();
}


bool Stack::IsBoolean( s32 idx )
{
    CheckIndex( idx );
    return IndexToValue( idx )->IsBoolean();
}


bool Stack::IsString( s32 idx )
{
    CheckIndex( idx );
    return IndexToValue( idx )->IsString();
}


bool Stack::IsFunction( s32 idx )
{
    CheckIndex( idx );
    return IndexToValue( idx )->IsFunction();
}


bool Stack::IsLightCFunction( s32 idx )
{
    CheckIndex( idx );
    return IndexToValue( idx )->IsLightCFunction();
}


bool Stack::IsCClosure( s32 idx )
{
    CheckIndex( idx );
    return IndexToValue( idx )->IsCClosure();
}


bool Stack::IsLuaClosure( s32 idx )
{
    CheckIndex( idx );
    return IndexToValue( idx )->IsLuaClosure();
}


bool Stack::IsTable( s32 idx )
{
    CheckIndex( idx );
    return IndexToValue( idx )->IsTable();
}


bool Stack::IsThread( s32 idx )
{
    CheckIndex( idx );
    return IndexToValue( idx )->IsThread();
}


}
