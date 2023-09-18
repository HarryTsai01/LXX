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


using namespace LOG;
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
    string strLine;
    std::getline( std::cin , strLine );
    //Log( LogCategory::Debugger , "%s" , strLine.c_str() );

    if( !ProcessCommand( strLine.c_str()) )
    {
        OnProcessCommand( strLine.c_str() );
    }
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
    _commandSystem.RegisterCommand("exit",this,&DebuggerStateBase::OnCommandExit );
    _commandSystem.RegisterCommand("version",this,&DebuggerStateBase::OnCommandVersion );
}


void DebuggerStateBase::OnUnRegisterCommand()
{

}


void DebuggerStateBase::OnCommandExit( const Array<LXX::String *> &Arguments )
{
    _stateMachine->RequestExitProgram();
}


void DebuggerStateBase::OnCommandVersion( const Array<LXX::String *> &Arguments )
{
    Log( LogCategory::Debugger , " Welcome to use LXX debugger, current version is %d.%d.%d.%d"
            , GLXXDebuggerVersion._major
            , GLXXDebuggerVersion._minor
            , GLXXDebuggerVersion._build
            , GLXXDebuggerVersion._revision
    );
}

}
} // LXX