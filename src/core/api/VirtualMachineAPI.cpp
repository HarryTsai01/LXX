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
    Value key("assert");
    Value value(LXX_Assert);
    globalTable->RawSetField(key, value);
}


}
