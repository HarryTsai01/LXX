//
// Created by Harry on 9/16/2023.
//

#include "DebuggerConnectionClient.h"

namespace LXX
{
namespace Debugger
{


bool ConnectionClient::Connect( Channel *channel )
{
    return channel->Connect();
}

}
} // LXX