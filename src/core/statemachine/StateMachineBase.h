//
// Created by Harry on 9/17/2023.
//

#ifndef XLUA_STATEMACHINEBASE_H
#define XLUA_STATEMACHINEBASE_H
#include <core/containers/UnorderedMap.h>
#include <cassert>
#include <core/log/log.h>

namespace LXX
{
namespace SM
{


class StateBase
{
public:
    StateBase() = default;
    virtual ~StateBase() = default;

    virtual const char* GetName() const = 0;
    virtual void OnEnter() = 0;
    virtual void OnUpdate() = 0;
    virtual void OnLeave() = 0;
};

template<typename StateEnum>
class StateMachineBase
{
public:
    StateMachineBase( StateEnum defaultState )
        : _bStartup ( false )
        , _defaultStateEnum(defaultState )
        , _currentStateEnum(defaultState )

    {

    }
    virtual ~StateMachineBase() = default;

    void Initialize()
    {
        OnRegisterState();
        OnInitialize();
    }

    void Destroy()
    {
        OnDestroy();
        OnUnRegisterState();
    }

    bool Startup()
    {
        assert( !_bStartup );
        _bStartup = true;
        _currentStateEnum = _defaultStateEnum;
        StateBase* currentState = _states[ _currentStateEnum ];
        currentState->OnEnter();

        LOG::Log( LOG::LogCategory::StateMachine , "[%s] Startup currentState:%s"
                  , GetName()
                  , currentState->GetName()
                  );
        return true;
    }


    void ChangeState( StateEnum newStateEnum )
    {
        StateBase* oldState = _states[ _currentStateEnum ];
        StateBase* newState = _states[ newStateEnum ];

        oldState->OnLeave();
        newState->OnEnter();

        LOG::Log( LOG::LogCategory::StateMachine , "[%s] ChangeState, oldState:%s , newState:%s"
                , GetName()
                , oldState->GetName()
                , newState->GetName()
        );

    }

    void Update()
    {
        StateBase *currentState = _states[ _currentStateEnum ];
        currentState->OnUpdate();
    }

    void Shutdown()
    {
        StateBase *currentState = _states[ _currentStateEnum ];
        currentState->OnLeave();
        _currentStateEnum = _defaultStateEnum;
    }

protected:
    virtual void OnRegisterState() = 0 ;
    virtual void OnUnRegisterState() = 0 ;
    virtual const char* GetName() = 0 ;
    virtual void OnInitialize() {}
    virtual void OnDestroy() {}

protected:
    bool _bStartup;
    StateEnum _defaultStateEnum;
    StateEnum _currentStateEnum;
    UnorderedMap< StateEnum , StateBase* > _states;
};
} // SM
} // LXX

#endif //XLUA_STATEMACHINEBASE_H
