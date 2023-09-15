//
// Created by Harry on 9/16/2023.
//

#ifndef XLUA_DEBUGGERCONNECTIONCLIENT_H
#define XLUA_DEBUGGERCONNECTIONCLIENT_H
#include "DebuggerConnection.h"

namespace LXX
{
namespace Debugger
{
class ConnectionClient : public Connection
{
public:
    ConnectionClient() = default;

    bool Connect( Channel *channel );

}; // ConnectionClient
} // Debugger
} // LXX

#endif //XLUA_DEBUGGERCONNECTIONCLIENT_H
