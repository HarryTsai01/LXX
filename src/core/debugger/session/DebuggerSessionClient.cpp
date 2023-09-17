//
// Created by Harry on 9/16/2023.
//

#include "DebuggerSessionClient.h"
#include <core/debugger/channel/DebuggerChannelSocket.h>
#include <core/debugger/channel/DebuggerChannelPipe.h>
#include <core/debugger/connection/DebuggerConnectionClient.h>

namespace LXX
{
namespace Debugger
{


bool SessionClient::OnInitialize()
{
    auto createChannel = [&]( ChannelType channelType )->Channel*
    {
        if( channelType == ChannelType::Pipe )
            return new ChannelPipeClient();
        else if( channelType == ChannelType::Socket )
            return new ChannelSocket();
        return nullptr;
    };

    _channel = createChannel( _channelType );
    auto clientConnection =  new ConnectionClient();
    _connection = clientConnection;
    return true;
}

bool SessionClient::Connect(  ConnectArgument* connectionArgument )
{
    ConnectionClient* clientConnection = GetConnection<ConnectionClient>();
    return clientConnection->Connect( _channel , connectionArgument );
}

void SessionClient::OnRegisterProtocolHandler()
{

}

}
} // LXX