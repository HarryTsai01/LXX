//
// Created by Harry on 9/17/2023.
//

#ifndef XLUA_DEFAULTSTATE_H
#define XLUA_DEFAULTSTATE_H
#include "StateBase.h"

namespace LXX
{
namespace Debugger
{
class DefaultState : public DebuggerStateBase
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    using DebuggerStateBase::DebuggerStateBase;
    virtual ~DefaultState() = default;
    virtual const char* GetName() const override { return "DebuggerDefaultState"; }
    virtual void OnEnter() override;
    virtual void OnUpdate() override;
    virtual void OnLeave() override;

private:
    virtual void OnProcessCommand( const char *command ) override;

protected:
    // >>> Commands
    virtual void OnRegisterCommand() override;
    virtual void OnUnRegisterCommand() override;
    // <<< Commands

}; //DefaultState
} //Debugger
} // LXX

#endif //XLUA_DEFAULTSTATE_H
