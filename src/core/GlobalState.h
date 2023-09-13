//
// Created by Harry on 8/4/2023.
//

#ifndef XLUA_GLOBALSTATE_H
#define XLUA_GLOBALSTATE_H
#include "State.h"
#include <core/Containers/Array.h>
#include <core/vm/ByteCodeChunk.h>
#include <core/objects/Table.h>

namespace LXX
{

class GlobalState
{
public:
    GlobalState();

    State *NewState();
    ByteCodeChunk *NewChunk( Array< String* > params );

    Table *GetGlobalTable() { return _globalTable; }
private:
    Array<State *> _states;
    Array< ByteCodeChunk *> _chunks;
    State *MainThread;
    Table *_globalTable;
};

}

#endif //XLUA_GLOBALSTATE_H
