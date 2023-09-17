//
// Created by Harry on 9/16/2023.
//

#include "DebuggerSession.h"
#include <core/debugger/channel/DebuggerChannelPipe.h>
#include <core/debugger/channel/DebuggerChannelSocket.h>
#include <core/debugger/connection/DebuggerConnectionClient.h>
#include <core/debugger/connection/DebuggerConnectionServer.h>

namespace LXX
{
namespace Debugger
{


Session::Session( ChannelType channelType )
        : _channelType( channelType )
        , _channel( nullptr )
        , _connection( nullptr )
{

}


bool Session::Initialize()
{
    if( !OnInitialize() )
        return false;

    _channel->BindOnChannelDisconnect(this, &Session::OnDisconnected );

    OnRegisterProtocolHandler();

    return true;
}


void Session::Destroy()
{
    UnRegisterAllProtocolHandler();
}


void Session::OnDisconnected()
{
    InvokeOnSessionClosed();
}


void Session::UnRegisterAllProtocolHandler()
{
    for( auto [_,protocolHandler] : _protocolHandlers)
    {
        delete protocolHandler;
    }
    _protocolHandlers.Clear();
}

} // Debugger
} // LXX