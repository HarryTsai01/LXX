//
// Created by Harry on 8/4/2023.
//

#ifndef XLUA_STACK_H
#define XLUA_STACK_H
#include "Value.h"
#include <core/CallInfo.h>
#include <core/exceptions/ExceptionInvalidType.h>
#include <core/objects/LuaClosure.h>
#include <core/Containers/Array.h>

namespace LXX
{

struct StackFrame
{
    StackFrame() = default;
    StackFrame( u32 base , u32 top )
     : _base(base)
     , _top(top)
    {

    }
    u32 _base;
    u32 _top;
    u32 _oldTop;
    u32 _localVariableStartIdx;
    u32 _temporaryVariableStartIdx;
    u32 _localAndTempVariableNum;
};

class Stack
{
public:
    Stack( u32 stackSize );

    bool NewFrame( const StackFrame& frame );
    bool RemoveFrame( StackFrame& outRemovedFrame );

    void Push(Value* val);
    void PushInteger(s32 val);
    void PushReal(f64 val);
    void PushLightUserData(void* val);
    void PushBoolean(bool val);
    void PushString(const char* val);
    void PushNil();
    void PushLightCFunction( CFunction func );
    void PushCClosure( CFunction func );
    void PushLuaClosure( LuaClosure* func );

    template<typename Function>
    void PushFunction( Function func );

    template<typename ...Args>
    void Push( Args ...args )
    {
        ( Push( args ), ...);
    }

    template<typename Arg>
    void Push( Arg )
    {
        assert( false );
    }

    void Push( s32 value )
    {
        PushInteger(value );
    }

    void Push( f64 value )
    {
        PushReal(value );
    }

    void Push( bool value )
    {
        PushBoolean(value );
    }

    void Pop( u32 n = 1 );
    bool IsNumber(  s32 idx );
    bool IsInteger( s32 idx );
    bool IsReal( s32 idx );
    bool IsLightUserData( s32 idx );
    bool IsBoolean( s32 idx );
    bool IsString( s32 idx );
    bool IsFunction( s32 idx );
    bool IsLightCFunction( s32 idx );
    bool IsCClosure( s32 idx );
    bool IsLuaClosure( s32 idx );
    bool IsTable( s32 idx );
    bool IsThread( s32 idx );

    void CheckIndex( s32 idx );
    void CheckIsNumber(  s32 idx );
    void CheckIsInteger( s32 idx );
    void CheckIsReal( s32 idx );
    void CheckIsLightUserData( s32 idx );
    void CheckIsBoolean( s32 idx );
    void CheckIsString( s32 idx );
    void CheckIsFunction( s32 idx );
    void CheckIsLightCFunction( s32 idx );
    void CheckIsCClosure( s32 idx );
    void CheckIsLuaClosure( s32 idx );
    void CheckIsTable( s32 idx );
    void CheckIsThread( s32 idx );

    Value* GetValue( s32 idx = -1 );
    s32 GetInteger( s32 idx = -1 , bool bCheck = false);
    f64 GetReal( s32 idx = -1 , bool bCheck = false);
    void* GetLightUserData( s32 idx = -1 , bool bCheck = false);
    bool GetBoolean( s32 idx = -1 , bool bCheck = false);
    String* GetString( s32 idx = -1 , bool bCheck = false);
    CFunction GetLightCFunction( s32 idx = -1 , bool bCheck = false);
    CFunction GetCClosure( s32 idx = -1, bool bCheck = false );
    LuaClosure* GetLuaClosure( s32 idx = -1 , bool bCheck = false);

    template<typename T>
    T As( s32 idx , bool bCheck = true );

    void SetValue( s32 destIdx , u32 srcIdx );
    void SetNil( s32 destIdx );
    u32 GetAbsIndex( s32 idx );
    Value* IndexToValue( s32 idx );
    u32 GetTop() const { return _top; }
    void SetTop( u32 top ) { _top = top; }
private:
    Array< Value > _data;
    Array< StackFrame > _frames;
    u32 _frameBase;
    u32 _frameTop;
    u32 _top;
};

template<>
s32 Stack::As<s32>( s32 idx , bool bCheck );

template<>
f64 Stack::As<f64>( s32 idx , bool bCheck );

template<>
Value* Stack::As<Value*>( s32 idx , bool bCheck );

template<>
void Stack::PushFunction( CFunction func );

template<>
void Stack::PushFunction( LuaClosure* func );


class FunctionCallScope
{
public:
    FunctionCallScope( State * state
                       , Stack *stack
                       , u32 functionIdx
                       , u32 argumentVariableNum
                       , u32 localVariableNum = 0
                       , u32 temporaryVariableNum = 0
                       );
    ~FunctionCallScope();
private:
    State *_state;
    Stack *_stack;
    u32 _functionIdx;
    u32 _argumentVariableNum;
    u32 _localVarNum;
    u32 _temporaryVariableNum;
};


}

#endif //XLUA_STACK_H
