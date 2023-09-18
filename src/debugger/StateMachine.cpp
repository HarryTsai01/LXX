//
// Created by Harry on 9/17/2023.
//

#include "StateMachine.h"
#include <core/statemachine/StateMachineBase.h>
#include "DefaultState.h"

namespace LXX
{
namespace Debugger
{


void StateMachine::OnRegisterState()
{
    _states =
    {
        { StateType::Default , new DefaultState( this ) },
    };

    LOG::SetLogLevel( LOG::LogLevel::Display );

    LOG::LogDebug( LOG::LogCategory::StateMachine ,
              "[%s] OnRegisterState"
                    , GetName()
    );
}


void StateMachine::OnUnRegisterState()
{
    for( auto [_,state] : _states )
    {
        delete state;
    }
    _states.Clear();

    LOG::LogDebug( LOG::LogCategory::StateMachine ,
              "[%s] OnUnRegisterState"
            , GetName()
    );
}


void StateMachine::OnInitialize()
{
    _vm.Startup();
}


void StateMachine::OnDestroy()
{
    _vm.Shutdown();
}



} // Debugger
} // LXX