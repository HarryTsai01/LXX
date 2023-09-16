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

    _connection->BindOnDisconnect(this, &Session::OnDisconnected );

    return true;
}


void Session::OnDisconnected()
{
    InvokeOnSessionClosed();
}

} // Debugger
} // LXX