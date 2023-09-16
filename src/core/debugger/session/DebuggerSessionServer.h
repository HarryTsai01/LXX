//
// Created by Harry on 9/16/2023.
//

#ifndef XLUA_DEBUGGERSESSIONSERVER_H
#define XLUA_DEBUGGERSESSIONSERVER_H
#include "DebuggerSession.h"

namespace LXX
{
namespace Debugger
{


class SessionServer : public Session
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    using Session::Session;
    virtual ~SessionServer() = default;
    virtual Role GetRole() const {  return Role::Server; }
private:
    virtual bool OnInitialize();
}; // SessionServer


} // Debugger
} // LXX

#endif //XLUA_DEBUGGERSESSIONSERVER_H
