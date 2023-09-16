//
// Created by Harry on 9/16/2023.
//

#include "F2BHello.h"

namespace LXX
{
namespace Debugger
{
namespace Protocol
{


void F2BHello::Serialize( IOStreamBase &stream )
{
    Base::Serialize( stream );
    if( stream.IsReading() )
    {
        stream >> _version;
    }
    else
    {
        _version = GLXXDebuggerVersion;
        stream << _version;
    }
}


}
}
} // LXX