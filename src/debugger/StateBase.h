//
// Created by Harry on 9/17/2023.
//

#ifndef XLUA_STATEBASE_H
#define XLUA_STATEBASE_H
#include <core/statemachine/StateMachineBase.h>
#include <core/utilities/CommandSystem.h>

namespace LXX
{
namespace Debugger
{


using namespace SM;
class StateMachine;
class DebuggerStateBase : public StateBase
{
public:
    DebuggerStateBase( StateMachine *stateMachine )
    : _stateMachine( stateMachine )
    {

    }
    virtual ~DebuggerStateBase() = default;

    virtual void OnEnter() override;
    virtual void OnUpdate() override;
    virtual void OnLeave() override;
private:
    void OnPreLogEvent();
    void OnPostLogEvent();
    void ShowIndicator();
    void NewLine();
protected:
    // >>> Commands
    virtual void OnProcessCommand( const char *command ) { }
    bool ProcessCommand( const char *command );
    virtual void OnRegisterCommand();
    virtual void OnUnRegisterCommand();
    void OnCommandExit(const Array< String *> & Arguments );
    void OnCommandVersion(const Array< String *> & Arguments );
    // <<< Commands
protected:
    StateMachine *_stateMachine;
    CommandSystem _commandSystem;
};


} // Debugger
} // LXX

#endif //XLUA_STATEBASE_H
