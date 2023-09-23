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
    : _parent( nullptr )
    , _scriptFileName( nullptr )
    ,_bBatchInstruction( false )
{

}


DebuggerSymbol::~DebuggerSymbol()
{

}


void DebuggerSymbol::AddLine( u32 lineNo , String *line )
{
    _lines.Add( lineNo , line );
}


void DebuggerSymbol::AddInstructionLine( u32 lineNo )
{
    if( !_bBatchInstruction )
        _instructionLines.Add( lineNo );
}


void DebuggerSymbol::DuplicateInstructionLine()
{
    if( !_bBatchInstruction )
        _instructionLines.Add( _instructionLines.GetLast() );
}


String* DebuggerSymbol::GetLine( u32 programCounter , u32 &lineNo )
{
    assert( programCounter < _instructionLines.Size() );
    lineNo = _instructionLines[ programCounter ];
    DebuggerSymbol *topMostParent = GetTopMostParent();
    return topMostParent->_lines[lineNo];
}


void DebuggerSymbol::BeginBatchInstruction( )
{
    _bBatchInstruction = true;
}


void DebuggerSymbol::EndBatchInstruction( u32 instructionNum, u32 lineNo )
{
    while( _instructionLines.Size() < instructionNum )
        _instructionLines.Add( lineNo );

    _bBatchInstruction = false;
}


void DebuggerSymbol::EndBatchInstructionDuplicate( u32 instructionNum )
{
    EndBatchInstruction( instructionNum , _instructionLines.GetLast() );
}

DebuggerSymbol *DebuggerSymbol::GetTopMostParent()
{
    DebuggerSymbol *topMostParent = this;
    while( topMostParent && topMostParent->_parent )
    {
        topMostParent = topMostParent->_parent;
    }
    return topMostParent;
}

#endif
}
} // LXX