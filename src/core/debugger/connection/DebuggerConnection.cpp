//
// Created by Harry on 9/16/2023.
//

#include "DebuggerConnection.h"

namespace LXX
{
namespace Debugger
{


bool Connection::Send( Channel *channel , Protocol::Base * protocol )
{
    return channel->Send( protocol );
}


bool Connection::Receive( Channel *channel , Protocol::Base *&protocol )
{
    return channel->Receive( protocol );
}



} // Debugger
} // LXX