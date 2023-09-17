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
    _session = new SessionClient( channelType );
    return _session->Initialize();
}


void Frontend::Destroy()
{
    if( _session )
    {
        _session->Destroy();
        _session = nullptr;
    }
}


}
} // LXX