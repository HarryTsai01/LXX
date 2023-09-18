//
// Created by Harry on 9/18/2023.
//

#ifndef XLUA_COMMANDSYSTEM_H
#define XLUA_COMMANDSYSTEM_H
#include <core/containers/UnorderedMap.h>
#include <core/delegate/Delegate.h>
#include <core/objects/string/String.h>

namespace LXX
{


DEFINE_DELEGATE_WITH_PARAMS(CommandEvent,void , const Array< String* >& );

class CommandSystem
{
public:

    CommandSystem() = default;
    CommandSystem(const CommandSystem& other) = delete;
    CommandSystem( CommandSystem&& other ) = delete;
    CommandSystem& operator=(const CommandSystem& other) = delete;
    CommandSystem& operator=(CommandSystem&& other) = delete;

    template<typename Class , typename Method>
    void RegisterCommand( const char* commandName , Class *Inst , Method method )
    {
        DelegateCommandEvent delegateCommandEvent;
        delegateCommandEvent.Bind( Inst , method );
        _commandEvents.Add(NEW_STRING( commandName ) , delegateCommandEvent );
    }

    template< typename Method >
    void RegisterCommand( const char* commandName , Method method )
    {
        DelegateCommandEvent delegateCommandEvent;
        delegateCommandEvent.Bind( method );
        _commandEvents.Add(NEW_STRING( commandName ) , delegateCommandEvent );
    }

    void UnRegisterCommand( const char *commandName )
    {
        _commandEvents.Remove(NEW_STRING( commandName ) );
    }


    bool Process( const char* commands );

private:
    UnorderedMap< String* ,DelegateCommandEvent > _commandEvents;
};


} // LXX

#endif //XLUA_COMMANDSYSTEM_H
