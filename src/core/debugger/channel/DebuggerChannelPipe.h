//
// Created by Harry on 9/16/2023.
//

#ifndef XLUA_DEBUGGERCHANNELPIPE_H
#define XLUA_DEBUGGERCHANNELPIPE_H
#include "DebuggerChannel.h"
#include <core/LXX.h>
#include <core/mem/MemoryAllocator.h>

namespace LXX
{
namespace Debugger
{
class ChannelPipe : public Channel
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    ChannelPipe() = default;

}; // ChannelPipe
}// Debugger
} // LXX

#endif //XLUA_DEBUGGERCHANNELPIPE_H
