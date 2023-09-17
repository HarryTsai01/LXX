//
// Created by Harry on 9/17/2023.
//

#ifndef XLUA_STATEBASE_H
#define XLUA_STATEBASE_H
#include <core/statemachine/StateMachineBase.h>

namespace LXX
{
namespace Debugger
{


using namespace SM;
class DebuggerStateBase : public StateBase
{
public:
    DebuggerStateBase() = default;
    virtual ~DebuggerStateBase() = default;

    virtual void OnEnter() override;
    virtual void OnUpdate() override;
    virtual void OnLeave() override;
private:
    void OnPreLogEvent();
    void OnPostLogEvent();
    void ShowIndicator();
    void NewLine();

};


} // Debugger
} // LXX

#endif //XLUA_STATEBASE_H
