//
// Created by Harry on 9/17/2023.
//
#include <core/LXX.h>
#include <core/log/log.h>
#include <iostream>
#include "StateMachine.h"

using namespace LXX;
using namespace LXX::LOG;
using namespace LXX::Debugger;

int main( int argc , const char ** argv )
{
    StateMachine::GetInstance().Initialize();
    StateMachine::GetInstance().Startup();

    while( !StateMachine::GetInstance().IsRequestExitProgram() )
    {
        StateMachine::GetInstance().Update();
    }
    StateMachine::GetInstance().Shutdown();
    StateMachine::GetInstance().Destroy();
    return 0;
}