//
// Created by Harry on 9/16/2023.
//

#ifndef XLUA_DEBUGGERSESSION_H
#define XLUA_DEBUGGERSESSION_H
#include <core/LXX.h>
#include <core/mem/MemoryAllocator.h>
#include <core/delegate/delegate.h>

namespace LXX
{
namespace Debugger
{

DEFINE_EVENT( OnSessionClosed );

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
    Session( ChannelType channelType );
    virtual ~Session() = default;

    bool Initialize();
    virtual Role GetRole() const =  0;

    template<typename ConnectionType>
    ConnectionType *GetConnection()
    {
        return dynamic_cast<ConnectionType *>(_connection);
    }

    DEFINE_CLASS_EVENT( OnSessionClosed );

protected:
    virtual bool OnInitialize() = 0;
private:
    void OnDisconnected();

protected:
    ChannelType _channelType;
    Channel *_channel;
    Connection *_connection;
}; // Session
} // Debugger
} // LXX

#endif //XLUA_DEBUGGERSESSION_H
