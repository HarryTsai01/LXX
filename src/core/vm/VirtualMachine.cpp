//
// Created by Harry on 8/5/2023.
//
#include "VirtualMachine.h"
#include <iostream>
#include <core/mem/MemoryAllocator.h>
#include <core/exceptions/ExceptionBase.h>
#include <core/log/log.h>


namespace LXX
{

using namespace LOG;

VirtualMachine::VirtualMachine()
    : _globalState( nullptr )
{
}


bool VirtualMachine::Startup()
{
    try
    {
        OnStartup();
    }
    catch ( ExceptionBase& e)
    {
        LogError(LogCategory::LXX,"[VirtualMachine] Startup Error : %s" , e.ToString()->GetData() );
        return false;
    }
    LogDebug( LogCategory::LXX , "[VirtualMachine] Startup " );
    return true;
}


void VirtualMachine::Shutdown()
{
    try
    {
        OnShutdown();
    }
    catch ( ExceptionBase& e)
    {
        LogError(LogCategory::LXX,"[VirtualMachine] Shutdown Error : %s" , e.ToString()->GetData() );
    }
    LogDebug( LogCategory::LXX , "[VirtualMachine] Shutdown " );
}


void VirtualMachine::OnStartup()
{
    _globalState = new GlobalState();
    RegisterInstructionSet();
    RegisterSystemFunction();
    RegisterAPI();
}


void VirtualMachine::OnShutdown()
{
}


bool VirtualMachine::Call(State *state, u32 nArgs )
{
    Stack& stack =  state->GetStack();
    s32 functionIdx = -nArgs - 1;

    if( !stack.IsFunction( functionIdx ) )
        ThrowError( state , "call a invalid function" );
    if( stack.IsLightCFunction( functionIdx ) )
    {
        FunctionCallScope scope( state
                , &stack
                , stack.GetAbsIndex(functionIdx )
                , nArgs
        );

        CFunction f = stack.GetLightCFunction( functionIdx, false );
        state->SetLastFunctionCallReturnValueCount(f( state ) );

    }
    else if( stack.IsLuaClosure( functionIdx ) )
    {
        LuaClosure *lc = stack.GetLuaClosure( functionIdx , false );
        FunctionCallScope scope( state
                , &stack ,
                                 stack.GetAbsIndex(functionIdx )
                , nArgs
                , lc->GetLocalVariableCount()
                , lc->GetTemporaryVariableCount()
        );

        state->SetLastFunctionCallReturnValueCount( CallLuaClosure( state , lc ) ); ;
    }
    else if( stack.IsCClosure( functionIdx ) )
    {
        ThrowError( state , "not implemented" );
    }

    return true;
}


bool VirtualMachine::ProtectCall( State *state , u32 nArgs )
{
    try
    {
        Call( state , nArgs );
    }
    catch ( ExceptionBase& e)
    {
        LogError(LogCategory::LXX,"[VirtualMachine] ProtectCall Error : %s" , e.ToString()->GetData() );
        return false;
    }
    return true;
}



}