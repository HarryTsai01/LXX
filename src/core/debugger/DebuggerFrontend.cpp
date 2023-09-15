//
// Created by Harry on 9/16/2023.
//

#include "DebuggerFrontend.h"

namespace LXX
{
namespace Debugger
{


Frontend::Frontend()
    : _session( nullptr )
{

}

bool Frontend::Startup( Session::ChannelType channelType )
{
    _session = new Session( channelType , Session::Role::Client );
    return _session->Initialize();
}


}
} // LXX