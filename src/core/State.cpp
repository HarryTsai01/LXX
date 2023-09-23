//
// Created by Harry on 8/5/2023.
//
#include "State.h"
#include <core/config/config.h>
#include <core/mem/MemoryAllocator.h>
#include <core/GlobalState.h>
#include <core/vm/ByteCodeChunk.h>
#include <iostream>
#include <core/log/log.h>


namespace LXX
{


State::State( VirtualMachine *vm  , GlobalState* G )
    : _vm( vm )
    , _stack( Config::STACK_SIZE )
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
    // process return Value
    // the return value would push into stack,
    // so we need to copy the return value from top to base(the function index start location)
    s32 curSrcValueIdx =  -_lastFunctionCallReturnValueCount;
    // the destination start location is the function index
    s32 curDestValueIdx = _currentCI->GetFunctionIdx();
    u32 processedReturnValueNum = 0;
    while( processedReturnValueNum < _lastFunctionCallReturnValueCount )
    {
        Value * curSrcValue =  _stack.IndexToValue( curSrcValueIdx );
        Value * curDestValue = _stack.IndexToValue( curDestValueIdx );

        *curDestValue = *curSrcValue;

        ++curDestValueIdx;
        ++curSrcValueIdx;
        ++ processedReturnValueNum;
    }

    // release local and temporary variables
    StackFrame lastFrame ;
    _stack.RemoveFrame( lastFrame );


    u32 curStackTop = _stack.GetTop();
    if( lastFrame._oldTop +  _lastFunctionCallReturnValueCount != curStackTop )
    {
        // just fixed the stack top in silence and print it out
        // we shouldn't throw any exception here ,because the EndFunctionCall come from the destructor of `FunctionCallScope`,
        // so if  we throw any exception here ,the exception can't be caught outside.

        LOG::LogError( LOG::LogCategory::LXX, "[STACK CHECK]Stack is out of balance after function call , Old stack top is %d , new stack top is %d , return value count is %d\n"
                , lastFrame._oldTop
                , curStackTop
                , _lastFunctionCallReturnValueCount
        );
        for( u32 i = lastFrame._oldTop ; i < curStackTop ; ++i )
        {
            Value * val = _stack.IndexToValue( i );
            LOG::LogError( LOG::LogCategory::LXX , "Stack[%d] = %s" , i , val->ToString()->GetData() );
        }
        _stack.SetTop( lastFrame._oldTop );
    }

    _stack.Pop( _currentCI->GetActualArgumentNum() + 1 );
    _currentCI =  _currentCI->GetPrevious();

}

}
