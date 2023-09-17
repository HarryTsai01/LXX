//
// Created by Harry on 9/16/2023.
//

#ifndef XLUA_DEBUGGERSESSIONCLIENT_H
#define XLUA_DEBUGGERSESSIONCLIENT_H
#include "DebuggerSession.h"
#include <core/debugger/connection/DebuggerConnectionClient.h>
#include <core/debugger/protocol/B2FWelcome.h>
#include <core/debugger/protocol/B2FCloseConnection.h>

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

    // >>> Protocol Handler
    DECLARE_PROTOCOL_HANDLER(B2FWelcome);
    DECLARE_PROTOCOL_HANDLER(B2FCloseConnection);
    // <<< Protocol Handler

};


} // Debugger
} // LXX

#endif //XLUA_DEBUGGERSESSIONCLIENT_H
