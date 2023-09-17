//
// Created by Harry on 9/16/2023.
//

#include "DebuggerSessionClient.h"
#include <core/debugger/channel/DebuggerChannelSocket.h>
#include <core/debugger/channel/DebuggerChannelPipe.h>
#include <core/debugger/connection/DebuggerConnectionClient.h>
#include <core/log/log.h>

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
    PROTOCOL_HANDLER_BEGIN()
        PROTOCOL_HANDLER(Protocol::ProtocolType::B2F_Welcome,Protocol::B2FWelcome,this, SessionClient::OnProtocol_B2FWelcome)
        PROTOCOL_HANDLER(Protocol::ProtocolType::B2F_CloseConnection,Protocol::B2FCloseConnection,this, SessionClient::OnProtocol_B2FCloseConnection)
    PROTOCOL_HANDLER_END()
}


IMPLEMENT_PROTOCOL_HANDLER( SessionClient , B2FWelcome)
{
    Version backendVersion = protocol->GetVersion();
    LOG::Log(LOG::LogCategory::Debugger ,
             "Frontend and backend have built connection, backend version:[%d.%d.%d.%d] "
              , backendVersion._major
              , backendVersion._minor
              , backendVersion._build
              , backendVersion._revision
              );
    if( backendVersion > GLXXDebuggerVersion )
    {
        LOG::LogWarning(LOG::LogCategory::Debugger ,
                        "the version of frontend is less than backend's, please update to date." );
    }
}


IMPLEMENT_PROTOCOL_HANDLER( SessionClient , B2FCloseConnection )
{
    LOG::LogCritical( LOG::LogCategory::Debugger ,
                     "Backend close connection ,the reason is %s",
                     protocol->GetReason()
            );

    Close();
}

}
} // LXX