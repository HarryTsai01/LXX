//
// Created by Harry on 9/2/2023.
//

#include "ByteCodeChunk.h"
#include <core/Value.h>
#include <core/State.h>
#include <core/vm/VirtualMachine.h>
#include <core/vm/Instructions/InstructionDecoder.h>

namespace LXX
{

ByteCodeChunk::ByteCodeChunk( Array< String* > parameters )
    : _parameters( std::move( parameters ) )
{
    _consts.Add();
}

u32 ByteCodeChunk::AddConstValue( s32 value )
{
    u32 foundIdx =  _consts.Find( Value(value ) );
    if( foundIdx == -1 )
    {
        foundIdx = _consts.GetSize() ;
        _consts.Add()->SetAsInt( value );
    }
    return foundIdx;
}


u32 ByteCodeChunk::AddConstValue( f64 value )
{
    u32 foundIdx =  _consts.Find( Value(value ) );
    if( foundIdx == -1 )
    {
        foundIdx = _consts.GetSize() ;
        _consts.Add()->SetAsReal( value );
    }
    return foundIdx;
}


u32 ByteCodeChunk::AddConstValue( bool value )
{
    u32 foundIdx =  _consts.Find( Value(value ) );
    if( foundIdx == -1 )
    {
        foundIdx = _consts.GetSize() ;
        _consts.Add()->SetAsBoolean( value );
    }
    return foundIdx;
}


u32 ByteCodeChunk::AddConstValue( const char* value )
{
    u32 foundIdx =  _consts.Find( Value(value ) );
    if( foundIdx == -1 )
    {
        foundIdx = _consts.GetSize() ;
        _consts.Add()->SetAsString( NEW_STRING( value ) );
    }
    return foundIdx;
}


u32 ByteCodeChunk::AddConstValue( String* value )
{
    u32 foundIdx =  _consts.Find( Value(value ) );
    if( foundIdx == -1 )
    {
        foundIdx = _consts.GetSize() ;
        _consts.Add()->SetAsString(  value );
    }
    return foundIdx;
}


u32 ByteCodeChunk::AddConstValue( LuaClosure* value )
{
    u32 foundIdx =  _consts.Find( Value(value ) );
    if( foundIdx == -1 )
    {
        foundIdx = _consts.GetSize() ;
        _consts.Add()->SetAsLuaClosure(  value );
    }
    return foundIdx;
}

const Value* ByteCodeChunk::GetConstValue( u32 idx ) const
{
    return &_consts[idx];
}

Value* ByteCodeChunk::GetConstValue( u32 idx )
{
    return &_consts[idx];
}


u32 ByteCodeChunk::AddTempVariable()
{
    String* newTempVarName = String::Format( "_LXX_TEMP_VAR_%d" , _tempVariables.Size() );
    _tempVariables.Add( newTempVarName );
    return _tempVariables.Size() - 1;
}


bool ByteCodeChunk::AddLocalVariable(String *name  , u32 *outIdx )
{
    if( IsLocalVariable( name  , outIdx ) ) return false;

    if( outIdx ) *outIdx = _localVariables.Size();
    _localVariables.Add( name );
    return true;
}

bool ByteCodeChunk::IsLocalVariable( String *name ,  u32 *outIdx )
{
    return VariableIsIn( _localVariables , name , outIdx );
}


bool ByteCodeChunk::IsParameterVariable(String *name , u32 *outIdx )
{
    return VariableIsIn( _parameters , name , outIdx );
}


bool ByteCodeChunk::VariableIsIn( Array< String* > &variables ,  String *name , u32 *outIdx )
{
    u32 index = variables.Find( [&]( String *str ) {
        if( str == name ) return true ;
        if( str->GetLength() != name->GetLength() ) return false ;

        return str->Compare( *name ) == 0;
    });
    if( outIdx ) *outIdx = index;
    return index != -1 ;
}


u32 ByteCodeChunk::GetDeclareArgumentVariableNum() const
{
    if( _parameters.Size() == 0 ) return 0 ;

    if( _parameters.GetLast()->Compare( "..." ) == 0 )
        return _parameters.Size() - 1;

    return _parameters.Size();
}

} // LXX