//
// Created by Harry on 9/16/2023.
//

#ifndef XLUA_DEBUGGERSESSION_H
#define XLUA_DEBUGGERSESSION_H
#include <core/LXX.h>
#include <core/mem/MemoryAllocator.h>
#include <core/delegate/delegate.h>
#include <core/containers/UnorderedMap.h>
#include <core/debugger/protocol/DebuggerProtocol.h>

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
    void Destroy();
    virtual Role GetRole() const =  0;

    template<typename ConnectionType>
    ConnectionType *GetConnection()
    {
        return dynamic_cast<ConnectionType *>(_connection);
    }

    DEFINE_CLASS_EVENT( OnSessionClosed );
protected:

#define PROTOCOL_HANDLER_BEGIN() _protocolHandlers = {
#define PROTOCOL_HANDLER( protocolType , protocolClass , ClassInst , ClassMethod ) { protocolType , \
        new Protocol::ProtocolHandlerImpl<protocolClass, std::remove_reference<decltype(*ClassInst)>::type ,decltype(&ClassMethod)>( \
            ClassInst,                                                                              \
            &ClassMethod ) } ,
#define PROTOCOL_HANDLER_END() };

#define DECLARE_PROTOCOL_HANDLER(protocolType) void OnProtocol_##protocolType( Protocol::protocolType *protocol );
#define IMPLEMENT_PROTOCOL_HANDLER( Class , protocolType) void Class::OnProtocol_##protocolType( Protocol::protocolType *protocol )

    protected:
    virtual bool OnInitialize() = 0;
    virtual void OnRegisterProtocolHandler() = 0 ;
private:
    void OnDisconnected();
    void UnRegisterAllProtocolHandler();

protected:
    ChannelType _channelType;
    Channel *_channel;
    Connection *_connection;
    UnorderedMap< Protocol::ProtocolType , Protocol::ProtocolHandler * > _protocolHandlers;
}; // Session
} // Debugger
} // LXX

#endif //XLUA_DEBUGGERSESSION_H
