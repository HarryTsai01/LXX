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

private:
    UnorderedMap< u32 ,  String * > _lines;
};


#endif
}
} // LXX

#endif //XLUA_DEBUGGERSYMBOL_H
