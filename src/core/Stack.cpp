//
// Created by Harry on 8/5/2023.
//
#include "Stack.h"
#include <core/mem/MemoryAllocator.h>
#include <core/config/config.h>
#include <core/State.h>


namespace LXX
{

Stack::Stack( u32 stackSize )
: _data(stackSize )
, _frameBase( 0 )
, _frameTop( stackSize )
, _top( 0 )
, _frames({ {0,stackSize } } )
{

}


bool Stack::NewFrame( const StackFrame& frame )
{
    _frameBase = frame._base;
    _frameTop = frame._top;
    if(  _frameTop > _data.Size() )
    {
        _data.AddNum( _frameTop - _data.Size() );
    }

    for( u32 i = frame._localVariableStartIdx ; i < _frameTop; ++i )
    {
        _data[ i ].SetAsNil();
    }

    // allocate local variables and temp variables
    _top += frame._localAndTempVariableNum;
    _frames.PushBack( frame );

    return true;
}


bool Stack::RemoveFrame( StackFrame& outRemovedFrame )
{
    if( _frames.Size() <= 1 )
        return false;

    // pop out current frame
    outRemovedFrame = _frames.PopBack();
    // release local variables and temp variables
    _top -= outRemovedFrame._localAndTempVariableNum;

    // restore to previous frame
    StackFrame prevFrame = _frames.GetLast();
    _frameBase = prevFrame._base;
    _frameTop  = prevFrame._top;

    return true;
}



void Stack::Push( Value *value )
{
    assert( _top + 1 < _frameTop && "Stack overflow" );
    _data[ _top ] = *value;
    ++_top;
}


void Stack::PushInteger(s32 val)
{
    assert( _top + 1 < _frameTop && "Stack overflow" );
    _data[ _top ].SetAsInt(val );
    ++_top;
}


void Stack::PushReal(f64 val)
{
    assert( _top + 1 < _frameTop && "Stack overflow" );
    _data[ _top ].SetAsReal(val );
    ++_top;
}


void Stack::PushLightUserData(void* val)
{
    assert( _top + 1 < _frameTop && "Stack overflow" );
    _data[ _top ].SetAsLightUserData(val );
    ++_top;
}


void Stack::PushBoolean(bool val)
{
    assert( _top + 1 < _frameTop && "Stack overflow" );
    _data[ _top ].SetAsBoolean(val );
    ++_top;
}


void Stack::PushString(const char* val)
{
    assert( _top + 1 < _frameTop && "Stack overflow" );
    _data[ _top ].SetAsString( NEW_STRING( val ) );
    ++_top;
}


void Stack::PushNil()
{
    assert( _top + 1 < _frameTop && "Stack overflow" );
    _data[ _top ].SetAsNil();
    ++_top;
}


void Stack::PushLightCFunction( CFunction func )
{
    assert( _top + 1 < _frameTop && "Stack overflow" );
    _data[ _top ].SetAsLightCFunction(func );
    ++_top;
}


void Stack::PushCClosure( CFunction func )
{
    assert( _top + 1 < _frameTop && "Stack overflow" );
    _data[ _top ].SetAsCClosure(func );
    ++_top;
}


void Stack::PushLuaClosure( LuaClosure *func )
{
    assert( _top + 1 < _frameTop && "Stack overflow" );
    _data[ _top ].SetAsLuaClosure( func );
    ++_top;
}

template<>
void Stack::PushFunction( CFunction func )
{
    PushLightCFunction( func );
}

template<>
void Stack::PushFunction( LuaClosure* func )
{
    PushLuaClosure( func );
}

void Stack::Pop( u32 n )
{
    assert( _top - n >= _frameBase && "Stack overflow" );
    _top -= n;
}


u32 Stack::GetAbsIndex( s32 idx )
{
    return idx >= 0 ?  _frameBase + idx : _top + idx;
}


Value *Stack::IndexToValue( s32 idx )
{
    if ( idx >= 0 )
    {
        assert( idx  < _top && "Stack overflow" );
        return & _data[ idx ];
    }
    else
    {
        assert( _top + idx >= _frameBase && "Stack overflow" );
        return & _data[ _top + idx ];
    }
}


void Stack::SetValue( s32 destIdx, u32 srcIdx )
{
    Value* dest = IndexToValue( destIdx );
    Value* src = IndexToValue( srcIdx );
    *dest = *src;
}


void Stack::SetNil( s32 destIdx )
{
    Value* dest = IndexToValue( destIdx );
    dest->SetAsNil();
}


FunctionCallScope::FunctionCallScope( State *state ,
                                      Stack *stack ,
                                      u32 functionIdx ,
                                      u32 argumentVariableNum ,
                                      u32 localVariableNum ,
                                      u32 temporaryVariableNum
                                      )
    : _state( state )
    , _stack( stack )
    , _functionIdx( functionIdx )
    , _argumentVariableNum( argumentVariableNum )
    , _localVarNum( localVariableNum )
    , _temporaryVariableNum( temporaryVariableNum )
{
    _state->BeginFunctionCall( functionIdx , argumentVariableNum , localVariableNum ,  temporaryVariableNum );
}


FunctionCallScope::~FunctionCallScope()
{
    u32 nActualResultNums = _state->GetCurrentCallInfo()->GetActualReturnValueNum();

    // copy return values to stack
    for( u32 i = 0 ; i < nActualResultNums ; i++ )
    {
        _stack->SetValue( i , -( i + 1 ) );
    }
    _state->EndFunctionCall();
}

}// namespace LXX
