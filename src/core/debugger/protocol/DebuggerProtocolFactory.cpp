//
// Created by Harry on 9/16/2023.
//

#include "DebuggerProtocolFactory.h"
#include "DebuggerProtocol.h"
#include "F2BHello.h"
namespace LXX
{
namespace Debugger
{
namespace Protocol
{

#define BEGIN_DEFINE_PROTOCOL() _protocols = {
#define DEFINE_PROTOCOL_ITEM( protocolType , protocolClass ) { static_cast<s32>( protocolType ) , new ProxyImpl< protocolClass , static_cast<s32>( protocolType )>() },
#define END_DEFINE_PROTOCOL() };

template<typename T , s32 protocolType>
Base* Factory::ProxyImpl<T,protocolType>::Create()
{
    return new T( static_cast<ProtocolType>( protocolType ) );
}

Factory::Factory()
{
    BEGIN_DEFINE_PROTOCOL()
        DEFINE_PROTOCOL_ITEM( ProtocolType::F2B_Hello , F2BHello )
    END_DEFINE_PROTOCOL()
}


Base* Factory::Create( s32 protocolType )
{
    auto It = _protocols.Find(protocolType );
    if( It == _protocols.End() )
        return nullptr;
    return It->Second->Create();
}

} //Protocol
} // Debugger
} // LXX
