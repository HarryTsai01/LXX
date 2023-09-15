//
// Created by Harry on 9/16/2023.
//

#ifndef XLUA_DEBUGGERFRONTEND_H
#define XLUA_DEBUGGERFRONTEND_H
#include <core/LXX.h>
#include <core/mem/MemoryAllocator.h>
#include <core/Singleton.h>
#include <core/debugger/DebuggerSession.h>

namespace LXX
{
namespace  Debugger
{

class Frontend : public Singleton<Frontend>
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL

public:
    bool Startup( Session::ChannelType channelType );
private:
    Frontend();
    Frontend(const Frontend&) = delete;
    Frontend(Frontend&&) = delete;
    Frontend& operator=(const Frontend&) = delete;
private:
    Session *_session;
};

} // Debugger
} // LXX

#endif //XLUA_DEBUGGERFRONTEND_H
