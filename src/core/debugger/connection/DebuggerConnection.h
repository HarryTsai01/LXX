//
// Created by Harry on 9/16/2023.
//

#ifndef XLUA_DEBUGGERCONNECTION_H
#define XLUA_DEBUGGERCONNECTION_H
#include <core/debugger/channel/DebuggerChannel.h>
#include <core/debugger/protocol/DebuggerProtocol.h>
#include <core/delegate/Delegate.h>

namespace LXX
{
namespace Debugger
{


DEFINE_EVENT( OnDisconnect );
DEFINE_EVENT( OnConnect );

class Connection
{
public:
    Connection() = default;
    virtual ~Connection() = default;

    bool Send( Channel * channel , Protocol::Base * protocol );
    bool Receive( Channel * channel , Protocol::Base * protocol );

    DEFINE_CLASS_EVENT( OnDisconnect );
    DEFINE_CLASS_EVENT( OnConnect );

}; // DebuggerConnection
} // Debugger


} // LXX

#endif //XLUA_DEBUGGERCONNECTION_H