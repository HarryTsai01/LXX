//
// Created by Harry on 9/17/2023.
//

#ifndef XLUA_STATEMACHINE_H
#define XLUA_STATEMACHINE_H
#include <core/Singleton.h>
#include <core/statemachine/StateMachineBase.h>
#include <core/vm/VirtualMachine.h>

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
    virtual void OnInitialize() override;
    virtual void OnDestroy() override;
    virtual const char* GetName() override
    {
        return "DebuggerStateMachine";
    }

    bool IsRequestExitProgram() const { return _requestExitProgram; }
    void RequestExitProgram() { _requestExitProgram = true ;}

    VirtualMachine& GetVM() { return _vm; }


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

    void RegisterCommands();
private:
    bool _requestExitProgram;
    VirtualMachine _vm;
};


} // Debugger
} // LXX

#endif //XLUA_STATEMACHINE_H
