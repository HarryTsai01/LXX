//
// Created by Harry on 9/2/2023.
//

#include "InstructionSetTestCase.h"
#include <core/vm/Instructions/InstructionEncoder.h>

namespace LXXTest
{

using namespace Instruction;

bool InstructionSetTestCase::Run()
{

    VirtualMachine vm;
    if( vm.Startup() )
    {
        GlobalState* _G = vm.GetGlobalState();
        State *state = _G->NewState();
        ByteCodeChunk* chunk = _G->NewChunk( {} );
        state->GetStack().PushLuaClosure( new LuaClosure( chunk ) );

        chunk->AddConstValue( 1 );
        chunk->AddConstValue( 2 );

        Array< u64 > &bytecode = chunk->GetCode();


        vm.Shutdown();

        return true;
    }

    return false;
}


} // LXXTest