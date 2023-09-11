//
// Created by Harry on 8/5/2023.
//
#include "Stack.h"
#include <core/exceptions/ExceptionInvalidType.h>
#include <core/exceptions/ExceptionInvalidStackIndex.h>
#include <core/exceptions/ExceptionInvalidParameter.h>

namespace LXX
{


void Stack::CheckIndex( s32 idx )
{
    if( idx >=0 )
    {
        if( _frameBase + idx >= _frameTop )
        {
            throw ExceptionInvalidStackIndex( idx );
        }
    }
    else
    {
        if( _top + idx < _frameBase )
        {
            throw ExceptionInvalidStackIndex( idx );
        }
    }
}


void Stack::CheckIsNumber(  s32 idx )
{
    CheckIndex( idx );
    Value *value = IndexToValue( idx );
    if( !value->IsNumber() )
        throw ExceptionInvalidParameter( idx , "Value is not a number" );

}


void Stack::CheckIsInteger( s32 idx )
{
    CheckIndex( idx );
    Value *value = IndexToValue( idx );
    if( !value->IsInteger() )
        throw ExceptionInvalidParameter( idx , "Value is not a Integer" );
}


void Stack::CheckIsReal( s32 idx )
{
    CheckIndex( idx );
    Value *value = IndexToValue( idx );
    if( !value->IsReal() )
        throw ExceptionInvalidParameter( idx , "Value is not a Real" );
}


void Stack::CheckIsLightUserData( s32 idx )
{
    CheckIndex( idx );
    Value *value = IndexToValue( idx );
    if( !value->IsLightUserData() )
        throw ExceptionInvalidParameter( idx , "Value is not a LightUserdata" );
}


void Stack::CheckIsBoolean( s32 idx )
{
    CheckIndex( idx );
    Value *value = IndexToValue( idx );
    if( !value->IsBoolean() )
        throw ExceptionInvalidParameter( idx , "Value is not a Boolean" );
}


void Stack::CheckIsString( s32 idx )
{
    CheckIndex( idx );
    Value *value = IndexToValue( idx );
    if( !value->IsString() )
        throw ExceptionInvalidParameter( idx , "Value is not a String" );
}


void Stack::CheckIsFunction( s32 idx )
{
    CheckIndex( idx );
    Value *value = IndexToValue( idx );
    if( !value->IsFunction() )
        throw ExceptionInvalidParameter( idx , "Value is not a Function" );
}


void Stack::CheckIsLightCFunction( s32 idx )
{
    CheckIndex( idx );
    Value *value = IndexToValue( idx );
    if( !value->IsLightCFunction() )
        throw ExceptionInvalidParameter( idx , "Value is not a Light C Function" );
}


void Stack::CheckIsCClosure( s32 idx )
{
    CheckIndex( idx );
    Value *value = IndexToValue( idx );
    if( !value->IsCClosure() )
        throw ExceptionInvalidParameter( idx , "Value is not a C Closure" );
}


void Stack::CheckIsLuaClosure( s32 idx )
{
    CheckIndex( idx );
    Value *value = IndexToValue( idx );
    if( !value->IsLuaClosure() )
        throw ExceptionInvalidParameter( idx , "Value is not a Lua Closure" );
}


void Stack::CheckIsTable( s32 idx )
{
    CheckIndex( idx );
    Value *value = IndexToValue( idx );
    if( !value->IsTable() )
        throw ExceptionInvalidParameter( idx , "Value is not a Table" );
}

void Stack::CheckIsThread( s32 idx )
{
    CheckIndex( idx );
    Value *value = IndexToValue( idx );
    if( !value->IsTable() )
        throw ExceptionInvalidParameter( idx , "Value is not a Thread" );
}


}
