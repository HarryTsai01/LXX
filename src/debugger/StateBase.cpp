//
// Created by Harry on 9/17/2023.
//

#include "StateBase.h"
#include "StateMachine.h"
#include <core/log/log.h>

namespace LXX
{
namespace Debugger
{

void DebuggerStateBase::OnEnter()
{
    OnRegisterCommand();
    LOG::GOnPreLogEvent.Bind( this , &DebuggerStateBase::OnPreLogEvent );
    LOG::GOnPostLogEvent.Bind( this , &DebuggerStateBase::OnPostLogEvent );
    ShowIndicator();
}


void DebuggerStateBase::OnLeave()
{
    LOG::GOnPreLogEvent.Unbind();
    LOG::GOnPostLogEvent.Unbind();
    OnUnRegisterCommand();
}


void DebuggerStateBase::OnUpdate()
{
    char buff[1024] = { 0 };
    std::cin >> buff;
    std::cout << buff << std::endl;

    ProcessCommand(buff);
}


void DebuggerStateBase::OnPreLogEvent()
{
    NewLine();
}


void DebuggerStateBase::OnPostLogEvent()
{
    ShowIndicator();
}


void DebuggerStateBase::ShowIndicator()
{
    std::cout << ">>>";
}


void DebuggerStateBase::NewLine()
{
    std::cout << std::endl;
}


bool DebuggerStateBase::ProcessCommand( const char *command )
{
    return _commandSystem.Process( command );
}


void DebuggerStateBase::OnRegisterCommand()
{
    _commandSystem.RegisterCommand("exit",this,&DebuggerStateBase::OnCommandExit);
}


void DebuggerStateBase::OnUnRegisterCommand()
{

}


void DebuggerStateBase::OnCommandExit( const Array<LXX::String *> &Arguments )
{
    _stateMachine->RequestExitProgram();
}

}
} // LXX