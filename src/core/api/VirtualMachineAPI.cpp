//
// Created by Harry on 9/13/2023.
//
#include <core/vm/VirtualMachine.h>
#include <core/api/API.h>

namespace LXX
{

void VirtualMachine::RegisterAPI()
{
    Table* globalTable = _globalState->GetGlobalTable();
    globalTable->RawSetField( Value("assert"), Value(LXX_Assert) );
    globalTable->RawSetField( Value("print"), Value(LXX_Print) );
    globalTable->RawSetField( Value("loadScript"), Value(LXX_LoadScript) );
}


}
