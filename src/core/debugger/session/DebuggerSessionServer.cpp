//
// Created by Harry on 9/16/2023.
//

#include "DebuggerSessionServer.h"
#include <core/debugger/connection/DebuggerConnectionServer.h>

namespace LXX
{
namespace Debugger
{


bool SessionServer::OnInitialize()
{
    auto serverConnection = new ConnectionServer();
    _connection = serverConnection;

    return serverConnection->Listen( _channel );
}


} // Debugger
} // LXX
