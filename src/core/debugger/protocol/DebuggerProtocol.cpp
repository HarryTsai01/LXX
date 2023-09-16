//
// Created by Harry on 9/16/2023.
//

#include "DebuggerProtocol.h"

namespace LXX
{
namespace Debugger
{
namespace Protocol
{


void Base::Serialize( IOStreamBase &stream )
{
    stream << _protocolType;
}


}
}
} // LXX