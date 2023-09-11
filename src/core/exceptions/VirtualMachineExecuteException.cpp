//
// Created by Harry on 8/5/2023.
//

#include "VirtualMachineExecuteException.h"

namespace LXX
{


VirtualMachineExecuteException::VirtualMachineExecuteException( const char *message )
    : _message( NEW_STRING( message ) )
{
}


String* VirtualMachineExecuteException::ToString() const
{
    return _message;
}

} // LXX