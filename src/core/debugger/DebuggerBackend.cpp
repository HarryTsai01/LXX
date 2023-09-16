//
// Created by Harry on 9/16/2023.
//

#include "DebuggerBackend.h"

namespace LXX
{
namespace Debugger
{


Backend::Backend()
: _session( nullptr )
{
}


bool Backend::Startup( Session::ChannelType channelType )
{
   _session = new SessionServer( channelType );
   return _session->Initialize();
}


}
} // LXX