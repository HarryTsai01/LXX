//
// Created by Harry on 9/22/2023.
//

#include "DebuggerUtil.h"
#include <core/objects/string/StringUtil.h>
namespace LXX
{
#if GENERATE_DEBUGGER_SYMBOL
namespace Debugger
{
namespace DebuggerUtil
{
String * GetBacktrace( State* state )
{
    String *backtrace = nullptr;
    CallInfo *ci = state->GetCurrentCallInfo();
    while( ci )
    {
        u32 programCounter = ci->GetProgramCounter();
        LuaClosure* lc = ci->GetLuaClosure();
        String *strLine = nullptr;
        u32 lineNo = -1;
        String *scriptFileName = nullptr;
        if( lc )
        {
            DebuggerSymbol *debuggerSymbol = lc->GetDebuggerSymbol();
            strLine = debuggerSymbol->GetLine( programCounter , lineNo );
            scriptFileName = debuggerSymbol->GetScriptFileName();
        }
        else
        {
            strLine = NEW_STRING("CFunction");
        }
        backtrace = StringUtil::Concat( backtrace ,
                                        String::Format("[%s:%d]%s\n"
                                                       , scriptFileName ? scriptFileName->GetData() : "MemoryString"
                                                       ,lineNo
                                                        ,strLine->GetData()
                                                       )
                                        );
        ci = ci->GetPrevious();
    }

    return backtrace;
}


String * GetSourceCodeLine( State* state )
{
    CallInfo *ci = state->GetCurrentCallInfo();
    LuaClosure* lc = ci->GetLuaClosure();
    DebuggerSymbol *debuggerSymbol = lc->GetDebuggerSymbol();
    u32 lineNo;
    String *strLine = debuggerSymbol->GetLine( ci->GetCurrentProgramCounter() , lineNo );
    return String::Format("%s:%d:%s"
                          , "MemoryString"
                          , lineNo
                          , strLine->GetData()
                          );
}
} // DebuggerUtil
} //debugger
#endif // GENERATE_DEBUGGER_SYMBOL
} // LXX
