//
// Created by Harry on 8/4/2023.
//

#ifndef XLUA_CALLINFO_H
#define XLUA_CALLINFO_H
#include "Value.h"
#include <assert.h>
#include <core/objects/GCObject.h>

namespace LXX
{

class State;
class StackFrame;
class CallInfo : public GCObject
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    CallInfo( CallInfo *previous
              , State *state
              , LuaClosure *luaClosure
              , u32 functionIdx
              , u32 actualArgumentVariableNum
              , u32 localVariableNum
              , u32 temporaryVariableNum
              );
    ~CallInfo() = default;

    CallInfo *GetPrevious() const { return _previous; }
    CallInfo *GetNext() const { return _next; }

    void SetNext( CallInfo *next ) { _next = next; }
    void SetPrevious( CallInfo *previous ) { _previous = previous; }

    void SetActualReturnValueNum(u32 actualResultNums) { _actualReturnValueNum = actualResultNums; }
    u32 GetActualReturnValueNum() const { return _actualReturnValueNum; }
    u32 GetActualArgumentNum() const { return _actualArgumentVariableNum; }

    void GetCurrentStackFrame( StackFrame * outFrame );
    u32 LocalVariableIndexToStackIndex( u32 localVariableIndex );
    u32 TemporaryVariableIndexToStackIndex( u32 temporaryVariableIndex );


    u32 GetLastFunctionCallReturnValueNum() const { return _lastFunctionCallReturnValueCount; }
    void SetLastFunctionCallReturnValueNum( u32 count ) { _lastFunctionCallReturnValueCount = count; }
    void SetProgramCounter( u32 pc ) { _programCounter = pc; }
    u32 GetProgramCounter() const { return _programCounter; }
    u32 GetCurrentProgramCounter() const { return _currentProgramCounter ; }
    LuaClosure *GetLuaClosure() const { return _luaClosure; }
    bool FetchNextInstruction(u64 &outInstruction );
    bool JumpOffset( s32 offset );

    u32 GetVariableArgument( u32 &startIdx , u32 &endIdx );
private:
    CallInfo *_previous;
    CallInfo *_next;
    State *_state;
    LuaClosure* _luaClosure;
    u32 _functionIdx;
    u32 _actualArgumentVariableNum;
    u32 _localVariableNum;
    u32 _temporaryVariableNum;
    u32 _actualReturnValueNum;
    u32 _localVariableStartIndex;
    u32 _temporaryVariableStartIndex;
    s32 _programCounter;
    s32 _currentProgramCounter;
    u32 _lastFunctionCallReturnValueCount;
    bool _bMarkAsReturn;
};


}
#endif //XLUA_CALLINFO_H
