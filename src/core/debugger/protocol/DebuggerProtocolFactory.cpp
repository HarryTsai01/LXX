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


Factory::Factory()
{
    _protocols =
    {
        { static_cast<s32>(ProtocolType::F2B_Hello) , new ProxyImpl<F2BHello>() },
    };
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
