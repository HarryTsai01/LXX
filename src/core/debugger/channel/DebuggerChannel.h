//
// Created by Harry on 9/16/2023.
//

#ifndef XLUA_DEBUGGERCHANNEL_H
#define XLUA_DEBUGGERCHANNEL_H
#include <core/objects/string/string.h>
#include <core/debugger/DebuggerCommunicateArgument.h>
#include <core/debugger/protocol/DebuggerProtocol.h>
#include <core/delegate/Delegate.h>

namespace LXX
{
namespace Debugger
{

DEFINE_EVENT( OnChannelDisconnect );
DEFINE_EVENT( OnChannelConnect );
DEFINE_EVENT( OnChannelSent );
DEFINE_EVENT( OnChannelReceive );

class Channel
{
public:
   virtual bool Initialize() { return false; }
   virtual void Close() { }
   virtual bool Send( Protocol::Base* protocol ) { return false; }
   virtual bool Receive( Protocol::Base*& protocol ) { return false; }
   virtual bool Bind( BindingArgument* argument ) { return false; }
   virtual bool Listen() { return false; }
   virtual Channel *Accept() { return nullptr; }
   virtual bool Connect( ConnectArgument* argument ) { return false; }

   DEFINE_CLASS_EVENT( OnChannelDisconnect );
   DEFINE_CLASS_EVENT( OnChannelConnect );
   DEFINE_CLASS_EVENT( OnChannelSent );
   DEFINE_CLASS_EVENT( OnChannelReceive );
}; // DebuggerChannel
} // Debugger
} // LXX

#endif //XLUA_DEBUGGERCHANNEL_H
