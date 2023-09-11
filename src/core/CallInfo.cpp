//
// Created by Harry on 8/5/2023.
//
#include "CallInfo.h"
#include "Stack.h"
#include "State.h"
#include <core/config/config.h>


namespace LXX
{


CallInfo::CallInfo( CallInfo *previous
    , State *state
    , LuaClosure *luaClosure
    , u32 functionIdx
    , u32 actualArgumentVariableNum
    , u32 localVariableNum
    , u32 temporaryVariableNum
    )
    : _previous( previous )
    , _next{ nullptr }
    , _state( state )
    , _luaClosure( luaClosure )
    , _functionIdx( functionIdx )
    , _actualArgumentVariableNum( actualArgumentVariableNum )
    , _localVariableNum( localVariableNum )
    , _temporaryVariableNum( temporaryVariableNum )
    , _actualReturnValueNum( 0 )
    , _localVariableStartIndex(functionIdx + 1 + actualArgumentVariableNum )
    , _temporaryVariableStartIndex( _localVariableStartIndex + localVariableNum )
    , _bMarkAsReturn( false )
    , _programCounter( 0 )
{
    if( _previous ) previous->_next = this;
}


void CallInfo::GetCurrentStackFrame( StackFrame * outFrame )
{
    outFrame->_base = _functionIdx;
    outFrame->_top = _functionIdx + _actualArgumentVariableNum + _localVariableNum + _temporaryVariableNum + Config::STACK_SIZE;
    u32 argumentVariableStartIndex = _functionIdx + 1;
    outFrame->_localVariableStartIdx = argumentVariableStartIndex + _actualArgumentVariableNum;
    outFrame->_temporaryVariableStartIdx = outFrame->_localVariableStartIdx + _localVariableNum;
}


u32 CallInfo::LocalVariableIndexToStackIndex( u32 localVariableIndex )
{
    return _localVariableStartIndex + localVariableIndex;
}


u32 CallInfo::TemporaryVariableIndexToStackIndex( u32 temporaryVariableIndex )
{
    return _temporaryVariableStartIndex + temporaryVariableIndex;
}


bool CallInfo::FetchNextInstruction(u64 &outInstruction )
{
    if( _bMarkAsReturn ) return false;

    ByteCodeChunk* chunk = _luaClosure->GetChunk();
    Array< u64 > & bytecode = chunk->GetCode();
    if( _programCounter >= bytecode.Size() )
    {
        return false;
    }
    outInstruction = bytecode[_programCounter];
    _programCounter++;

    return true;
}


bool CallInfo::JumpOffset( s32 offset )
{
    ByteCodeChunk* chunk = _luaClosure->GetChunk();
    Array< u64 > & bytecode = chunk->GetCode();
    if( _programCounter + offset >= bytecode.Size()
    || _programCounter + offset < 0 )
        return false;
    _programCounter += offset;
    return true;
}


u32 CallInfo::GetVariableArgument( u32 &startIdx , u32 &endIdx )
{
    u32 declareArgumentNum = _luaClosure->GetChunk()->GetDeclareArgumentVariableNum();

    if( _luaClosure == nullptr )
        return 0;
    if( _actualArgumentVariableNum <= declareArgumentNum )
        return 0;

    u32 argumentStartIdx = _functionIdx + 1;
    startIdx = argumentStartIdx + declareArgumentNum;
    endIdx = argumentStartIdx + _actualArgumentVariableNum - 1 ;

    return _actualArgumentVariableNum - declareArgumentNum;
}

}
