//
// Created by Harry on 9/16/2023.
//

#ifndef XLUA_DEBUGGERBACKEND_H
#define XLUA_DEBUGGERBACKEND_H
#include <core/LXX.h>
#include <core/mem/MemoryAllocator.h>
#include <core/Singleton.h>
#include <core/debugger/DebuggerSession.h>

namespace LXX
{
namespace Debugger
{
class Backend :  public Singleton<Backend>
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    bool Startup( Session::ChannelType channelType );
private:
    Backend();
    Backend( const Backend& ) = delete;
    Backend( Backend&& ) = delete;
    ~Backend() = default;
private:
    Session *_session;
};
} // Debugger
} // LXX

#endif //XLUA_DEBUGGERBACKEND_H
