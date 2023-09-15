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


Session::Session( ChannelType channelType, Role role )
        : _channelType( channelType )
        , _role( role )
        , _channel( nullptr )
        , _connection( nullptr )
{

}


bool Session::Initialize()
{
    auto createChannel = [&]( ChannelType channelType )->Channel*
    {
        if( channelType == ChannelType::Pipe )
            return new ChannelPipe();
        else if( channelType == ChannelType::Socket )
            return new ChannelSocket();
        return nullptr;
    };
    _channel = createChannel( _channelType );
    if( _role == Role::Client )
    {
        auto clientConnection = new ConnectionClient();
        _connection = clientConnection;
        return clientConnection->Connect( _channel );
    }
    else
    {
        auto serverConnection = new ConnectionServer();
        _connection = serverConnection;
        return serverConnection->Listen( _channel );
    }
}

} // Debugger
} // LXX