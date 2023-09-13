//
// Created by Harry on 8/5/2023.
//
#include "VirtualMachine.h"
#include <iostream>
#include <core/mem/MemoryAllocator.h>
#include <core/exceptions/ExceptionBase.h>


namespace LXX
{


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
        std::cout << e.ToString() << std::endl;
        return false;
    }
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
        std::cout << e.ToString() << std::endl;
    }
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

bool VirtualMachine::ProtectCall( State *state , u32 nArgs )
{
    try
    {
        Stack& stack =  state->GetStack();
        s32 functionIdx = -nArgs - 1;

        if( !stack.IsFunction( functionIdx ) )
            ThrowError( "call a invalid function" );
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
            ThrowError( "not implemented" );
        }
    }
    catch ( ExceptionBase& e)
    {
        std::cout << e.ToString() << std::endl;
        return false;
    }
    return true;
}



}