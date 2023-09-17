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
        { StateType::Default , new DefaultState() },
    };

    LOG::Log( LOG::LogCategory::StateMachine ,
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

    LOG::Log( LOG::LogCategory::StateMachine ,
              "[%s] OnUnRegisterState"
            , GetName()
    );
}


} // Debugger
} // LXX