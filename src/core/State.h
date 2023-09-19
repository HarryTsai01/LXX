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
#include <core/exceptions/VirtualMachineExecuteException.h>

namespace LXX
{

using namespace Instruction;

class GlobalState;

class State : public GCObject
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    State( VirtualMachine *vm , GlobalState *G );

    Stack& GetStack() { return _stack ; }


    void BeginFunctionCall( u32 func , u32 argumentVariableNum , u32 localVariableNum , u32 temporaryVariableNum );
    void EndFunctionCall();

    CallInfo* GetCurrentCallInfo() { return _currentCI; }
    bool FetchNextInstruction(u64 &outInstruction ) { return _currentCI->FetchNextInstruction(outInstruction); }

    u32 GetLastFunctionCallReturnValueCount() const { return _lastFunctionCallReturnValueCount; }
    void SetLastFunctionCallReturnValueCount( u32 count ) { _lastFunctionCallReturnValueCount = count; }

    GlobalState* GetGlobalState() { return _G; }
    VirtualMachine* GetVM() { return _vm; }

    template<typename ...Args>
    void ThrowError( const char *format, Args ...args )
    {
        throw VirtualMachineExecuteException( format , std::forward<Args>(args) ... );
    }
private:
    VirtualMachine *_vm;
    Stack _stack;
    GlobalState *_G;
    CallInfo *_currentCI;
    u32 _lastFunctionCallReturnValueCount;
}; // class State


} // namespace LXX

#endif //XLUA_STATE_H
