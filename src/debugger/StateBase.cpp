//
// Created by Harry on 9/17/2023.
//

#include "StateBase.h"
#include <core/log/log.h>

namespace LXX
{
namespace Debugger
{

void DebuggerStateBase::OnEnter()
{
    LOG::GOnPreLogEvent.Bind( this , &DebuggerStateBase::OnPreLogEvent );
    LOG::GOnPostLogEvent.Bind( this , &DebuggerStateBase::OnPostLogEvent );
    std::cout << ">>>";
}


void DebuggerStateBase::OnLeave()
{

}


void DebuggerStateBase::OnUpdate()
{
    char buff[1024] = { 0 };
    std::cin >> buff;
    std::cout << buff << std::endl;
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

}
} // LXX