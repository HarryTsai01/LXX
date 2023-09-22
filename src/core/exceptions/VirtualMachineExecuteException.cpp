//
// Created by Harry on 8/5/2023.
//

#include "VirtualMachineExecuteException.h"
#include <core/State.h>
#include <core/objects/LuaClosure.h>
#include <core/objects/string/StringUtil.h>
#include <core/debugger/DebuggerUtil.h>

namespace LXX
{


VirtualMachineExecuteException::VirtualMachineExecuteException( State* state , String *message )
    : _state( state )
    , _message(  message )
{
    _backtrace = Debugger::DebuggerUtil::GetBacktrace( state );
}


String* VirtualMachineExecuteException::ToString() const
{
#if GENERATE_DEBUGGER_SYMBOL
    return String::Format( "[VirtualMachineExecuteException] %s \n backtrace:\n%s"
                           , _message->GetData()
                           , _backtrace->GetData()
                           );
#else
    return String::Format( "[VirtualMachineExecuteException] %s", _message->GetData() );
#endif
}

} // LXX