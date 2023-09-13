//
// Created by Harry on 8/5/2023.
//
#include "State.h"
#include <core/config/config.h>
#include <core/mem/MemoryAllocator.h>
#include <core/GlobalState.h>
#include <core/vm/ByteCodeChunk.h>


namespace LXX
{


State::State( GlobalState* G )
    : _stack( Config::STACK_SIZE )
    , _G( G )
    , _currentCI( nullptr )
    , _lastFunctionCallReturnValueCount( 0 )
{
}




void State::BeginFunctionCall( u32 functionIdx , u32 argumentVariableNum , u32 localVariableNum, u32 temporaryVariableNum  )
{
    _lastFunctionCallReturnValueCount = 0;

    LuaClosure *closure = nullptr;
    if( _stack.IsLuaClosure( functionIdx ) )
    {
        closure = _stack.GetLuaClosure( functionIdx );
    }
    CallInfo *newCI = new CallInfo( _currentCI
            , this
            , closure
            , functionIdx
            , argumentVariableNum
            , localVariableNum
            , temporaryVariableNum
            );

    newCI->SetPrevious( _currentCI );
    if( _currentCI ) _currentCI->SetNext( newCI );
    _currentCI = newCI;

    StackFrame frame;
    newCI->GetCurrentStackFrame( &frame );
    _stack.NewFrame( frame );
}


void State::EndFunctionCall()
{
    StackFrame lastFrame ;
    _stack.RemoveFrame( lastFrame );

    u32 curStackTop = _stack.GetTop();
    if( lastFrame._oldTop +  _lastFunctionCallReturnValueCount != curStackTop )
        ThrowError( "Stack is out of balance after function call , Old stack top is %d , new stack top is %d , return value count is %d"
                , lastFrame._oldTop
                , curStackTop
                , _lastFunctionCallReturnValueCount
        );

    _currentCI =  _currentCI->GetPrevious();

}

}
