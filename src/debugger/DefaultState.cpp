//
// Created by Harry on 9/17/2023.
//

#include "DefaultState.h"
#include "StateMachine.h"
#include <core/exceptions/ExceptionBase.h>

namespace LXX
{
namespace Debugger
{

using namespace LOG;

void DefaultState::OnEnter()
{
    DebuggerStateBase::OnEnter();
    ProcessCommand("version" );
}


void DefaultState::OnLeave()
{
    DebuggerStateBase::OnLeave();
}


void DefaultState::OnUpdate()
{
    DebuggerStateBase::OnUpdate();
}


void DefaultState::OnRegisterCommand()
{
    DebuggerStateBase::OnRegisterCommand();
}


void DefaultState::OnUnRegisterCommand()
{
    DebuggerStateBase::OnUnRegisterCommand();
}


void DefaultState::OnProcessCommand( const char *command )
{
    VirtualMachine& vm = _stateMachine->GetVM();

    try
    {
        vm.Execute( command );
    } catch ( ExceptionBase &e )
    {
        LogWarning( LogCategory::Debugger , "%s", e.ToString()->GetData() );
    }
}


}
} // LXX