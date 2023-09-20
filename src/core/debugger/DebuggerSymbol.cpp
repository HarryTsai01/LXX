//
// Created by Harry on 9/21/2023.
//

#include "DebuggerSymbol.h"

namespace LXX
{
namespace Debugger
{


#if GENERATE_DEBUGGER_SYMBOL
DebuggerSymbol::DebuggerSymbol()
{

}


DebuggerSymbol::~DebuggerSymbol()
{

}


void DebuggerSymbol::AddLine( u32 lineNo , String *line )
{
    _lines.Add( lineNo , line );
}
#endif
}
} // LXX