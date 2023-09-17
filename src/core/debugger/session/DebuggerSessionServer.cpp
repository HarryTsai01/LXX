//
// Created by Harry on 9/16/2023.
//

#include "DebuggerSessionServer.h"
#include <core/debugger/connection/DebuggerConnectionServer.h>
#include <core/debugger/protocol/B2FWelcome.h>
#include <core/debugger/protocol/B2FCloseConnection.h>

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


void SessionServer::OnRegisterProtocolHandler()
{
    PROTOCOL_HANDLER_BEGIN()
        PROTOCOL_HANDLER( Protocol::ProtocolType::F2B_Hello, Protocol::F2BHello ,this,SessionServer::OnProtocol_F2BHello )
    PROTOCOL_HANDLER_END()
}


IMPLEMENT_PROTOCOL_HANDLER( SessionServer,F2BHello )
{
    auto frontendVersion = protocol->GetVersion();
    if( GLXXDebuggerVersion < frontendVersion )
    {
        _channel->Send( new Protocol::B2FCloseConnection( Protocol::B2FCloseConnection::Reason::FrontendVersionGreaterThanBackend ) );
    }
    else
    {
        _channel->Send( new Protocol::B2FWelcome() );
    }
}


} // Debugger
} // LXX
