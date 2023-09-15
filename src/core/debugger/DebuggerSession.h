//
// Created by Harry on 9/16/2023.
//

#ifndef XLUA_DEBUGGERSESSION_H
#define XLUA_DEBUGGERSESSION_H
#include <core/LXX.h>
#include <core/mem/MemoryAllocator.h>

namespace LXX
{
namespace Debugger
{


class Channel;
class Connection;
class Session
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    enum class ChannelType
    {
        Pipe,
        Socket
    };
    enum class Role
    {
        Client,
        Server
    };
    Session( ChannelType channelType, Role role );

    bool Initialize();
    Role GetRole() const { return _role; }

private:
    ChannelType _channelType;
    Role _role;
    Channel *_channel;
    Connection *_connection;
}; // Session
} // Debugger
} // LXX

#endif //XLUA_DEBUGGERSESSION_H
