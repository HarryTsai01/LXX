//
// Created by Harry on 9/16/2023.
//

#include "DebuggerConnectionServer.h"

namespace LXX
{
namespace Debugger
{


bool ConnectionServer::Listen( Channel *channel )
{
    return channel->Listen( );
}


Channel *Accept( Channel *channel )
{
    return channel->Accept( );
}


}
} // LXX