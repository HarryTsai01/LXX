//
// Created by Harry on 9/16/2023.
//

#ifndef XLUA_DEBUGGERCONNECTIONSERVER_H
#define XLUA_DEBUGGERCONNECTIONSERVER_H
#include "DebuggerConnection.h"

namespace LXX
{
namespace Debugger
{
class ConnectionServer : public Connection
{
public:
    ConnectionServer() = default;

    bool Listen( Channel *channel ) ;
    Channel *Accept( Channel *channel ) ;

}; // ConnectionServer
} // Debugger

} // LXX

#endif //XLUA_DEBUGGERCONNECTIONSERVER_H
