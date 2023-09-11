//
// Created by Harry on 8/4/2023.
//

#ifndef XLUA_STATE_H
#define XLUA_STATE_H

#include <cstddef>
#include "CallInfo.h"
#include "Stack.h"
#include <core/objects/GCObject.h>
#include <core/vm/Instructions/InstructionSet.h>

namespace LXX
{

using namespace Instruction;

class GlobalState;

class State : public GCObject
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    State( GlobalState *G );

    Stack& GetStack() { return _stack ; }


    void BeginFunctionCall( u32 func , u32 argumentVariableNum , u32 localVariableNum , u32 temporaryVariableNum );
    void EndFunctionCall();

    CallInfo* GetCurrentCallInfo() { return _currentCI; }
    bool FetchNextInstruction(u64 &outInstruction ) { return _currentCI->FetchNextInstruction(outInstruction); }

    u32 GetLastFunctionCallReturnValueCount() const { return _lastFunctionCallReturnValueCount; }
    void SetLastFunctionCallReturnValueCount( u32 count ) { _lastFunctionCallReturnValueCount = count; }
private:
    Stack _stack;
    GlobalState *_G;
    CallInfo *_currentCI;
    u32 _lastFunctionCallReturnValueCount;
}; // class State


} // namespace LXX

#endif //XLUA_STATE_H
