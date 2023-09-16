//
// Created by Harry on 9/16/2023.
//

#ifndef XLUA_DEBUGGERCHANNELSOCKET_H
#define XLUA_DEBUGGERCHANNELSOCKET_H
#include "DebuggerChannel.h"
#include <core/LXX.h>
#include <core/mem/MemoryAllocator.h>
namespace LXX
{
namespace Debugger
{


class ChannelSocket : public Channel
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    ChannelSocket();

}; // ChannelSocket


} // Debugger
} // LXX

#endif //XLUA_DEBUGGERCHANNELSOCKET_H
