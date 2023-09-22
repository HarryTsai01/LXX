//
// Created by Harry on 9/22/2023.
//

#ifndef XLUA_DEBUGGERUTIL_H
#define XLUA_DEBUGGERUTIL_H
#include <core/LXX.h>
#include <core/State.h>
#include <core/objects/string/String.h>

namespace LXX
{
#if GENERATE_DEBUGGER_SYMBOL
namespace Debugger
{
namespace DebuggerUtil
{
String * GetBacktrace( State* state );
String * GetSourceCodeLine(State * state );
} // DebuggerUtil
} //debugger
#endif // GENERATE_DEBUGGER_SYMBOL
} // LXX

#endif //XLUA_DEBUGGERUTIL_H
