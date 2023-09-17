//
// Created by Harry on 9/17/2023.
//

#ifndef XLUA_STATEMACHINE_H
#define XLUA_STATEMACHINE_H
#include <core/Singleton.h>
#include <core/statemachine/StateMachineBase.h>

namespace LXX
{
namespace Debugger
{
using namespace SM;

enum class StateType
{
    Default,
};

class StateMachine : public StateMachineBase< StateType > , public Singleton<StateMachine>
{
    friend class Singleton<StateMachine>;
public:
    virtual void OnRegisterState() override;;
    virtual void OnUnRegisterState() override;
    virtual const char* GetName() override
    {
        return "DebuggerStateMachine";
    }

    bool IsRequestExitProgram() const { return false; }
    void RequestExitProgram() { _requestExitProgram = true ;}

private:
    StateMachine()
     : StateMachineBase<StateType>(StateType::Default )
     ,_requestExitProgram (false )
    {

    }
    StateMachine(const StateMachine& other) = delete;
    StateMachine(StateMachine&& other) = delete;
    StateMachine& operator=(const StateMachine& other) = delete;
    StateMachine& operator=(const StateMachine&& other) = delete;


private:
    bool _requestExitProgram;

};


} // Debugger
} // LXX

#endif //XLUA_STATEMACHINE_H
