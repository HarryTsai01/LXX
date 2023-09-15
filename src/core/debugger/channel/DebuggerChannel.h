//
// Created by Harry on 9/16/2023.
//

#ifndef XLUA_DEBUGGERCHANNEL_H
#define XLUA_DEBUGGERCHANNEL_H

namespace LXX
{
namespace Debugger
{
class Channel
{
public:
   virtual bool Listen()  { return false ;}
   virtual bool Connect() { return false ;}
   virtual Channel* Accept() { return nullptr ; }
   virtual void Close() { }
}; // DebuggerChannel
} // Debugger
} // LXX

#endif //XLUA_DEBUGGERCHANNEL_H
