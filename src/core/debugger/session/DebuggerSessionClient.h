//
// Created by Harry on 9/16/2023.
//

#ifndef XLUA_DEBUGGERSESSIONCLIENT_H
#define XLUA_DEBUGGERSESSIONCLIENT_H
#include "DebuggerSession.h"
#include <core/debugger/connection/DebuggerConnectionClient.h>

namespace LXX
{
namespace Debugger
{

class SessionClient : public Session
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    using Session::Session;
    virtual ~SessionClient() = default;
    virtual Role GetRole() const {  return Role::Client; }

    bool Connect( ConnectArgument* connectionArgument );
private:
    virtual bool OnInitialize() override;
    virtual void OnRegisterProtocolHandler() override;

};


} // Debugger
} // LXX

#endif //XLUA_DEBUGGERSESSIONCLIENT_H
