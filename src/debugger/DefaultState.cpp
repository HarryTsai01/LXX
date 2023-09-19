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
    // for test
    OnProcessCommand("loadScript('main')" );
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
    _commandSystem.RegisterCommand("chunk",this,&DefaultState::OnCommandChunk );
}


void DefaultState::OnUnRegisterCommand()
{
    _commandSystem.UnRegisterCommand("chunk" );
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
        LogError( LogCategory::Debugger , "%s", e.ToString()->GetData() );
    }
}


// >>> chunk command
void DefaultState::PrintChunkHelpCommand()
{
    LogDisplay( LogCategory::Debugger ,
                "chunk command is used to view or disassembly any specific LuaClosure .\n"
                        " chunk list : show all chunks with chunk-id \n"
                );
}


void DefaultState::OnCommandChunk( const Array<LXX::String *> &Arguments )
{
    if( Arguments.Size() <= 1 )
    {
        PrintChunkHelpCommand();
        return;
    }

    if( Arguments[1]->Compare("list") == 0 )
    {

    }
    else
    {
        LogError(LogCategory::Debugger,"unknown chunk command :%s", Arguments[0]->GetData() );
    }

}
// <<< chunk command


}
} // LXX