//
// Created by Harry on 9/21/2023.
//

#ifndef XLUA_DEBUGGERSYMBOL_H
#define XLUA_DEBUGGERSYMBOL_H
#include <core/LXX.h>
#include <core/objects/GCObject.h>
#include <core/objects/string/String.h>
#include <core/containers/Array.h>
#include <core/containers/UnorderedMap.h>

namespace LXX
{
namespace Debugger
{
#if GENERATE_DEBUGGER_SYMBOL


class DebuggerSymbol : public GCObject
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    DebuggerSymbol();
    virtual ~DebuggerSymbol();

    void AddLine( u32 lineNo , String* line );

    void AddInstructionLine( u32 lineNo );
    void DuplicateInstructionLine();

    String* GetLine(u32 programCounter , u32 &lineNo );
    u32 GetInstructionLineNum() const { return _instructionLines.Size() ; }
    void BeginBatchInstruction();
    void EndBatchInstruction( u32 instructionNum  , u32 linNo );
    void EndBatchInstructionDuplicate( u32 instructionNum  );
    bool GetIsBatchInstruction() const { return _bBatchInstruction ; }

private:
    UnorderedMap< u32 ,  String * > _lines;
    Array< u32 > _instructionLines;
    bool _bBatchInstruction;
};


#endif
}
} // LXX

#endif //XLUA_DEBUGGERSYMBOL_H
