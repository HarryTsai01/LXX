//
// Created by Harry on 9/17/2023.
//

#include "DefaultState.h"

namespace LXX
{
namespace Debugger
{

using namespace LOG;

void DefaultState::OnEnter()
{
    DebuggerStateBase::OnEnter();
    Log( LogCategory::Debugger , " Welcome to use LXX debugger, current version is %d.%d.%d.%d"
            , GLXXDebuggerVersion._major
            , GLXXDebuggerVersion._minor
            , GLXXDebuggerVersion._build
            , GLXXDebuggerVersion._revision
    );
}


void DefaultState::OnLeave()
{
    DebuggerStateBase::OnLeave();
}


void DefaultState::OnUpdate()
{
    DebuggerStateBase::OnUpdate();
}


}
} // LXX