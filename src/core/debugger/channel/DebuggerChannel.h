//
// Created by Harry on 9/16/2023.
//

#ifndef XLUA_DEBUGGERCHANNEL_H
#define XLUA_DEBUGGERCHANNEL_H
#include <core/objects/string/string.h>
#include <core/debugger/DebuggerCommunicateArgument.h>
#include <core/debugger/protocol/DebuggerProtocol.h>

namespace LXX
{
namespace Debugger
{


class Channel
{
public:
   virtual bool Initialize() { return false; }
   virtual void Close() { }
   virtual bool Send( Protocol::Base* protocol ) { return false; }
   virtual bool Receive( Protocol::Base* protocol ) { return false; }
   virtual bool Bind( BindingArgument* argument ) { return false; }
   virtual bool Listen() { return false; }
   virtual Channel *Accept() { return nullptr; }
   virtual bool Connect( ConnectArgument* argument ) { return false; }
}; // DebuggerChannel
} // Debugger
} // LXX

#endif //XLUA_DEBUGGERCHANNEL_H
