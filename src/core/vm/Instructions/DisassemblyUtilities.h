//
// Created by Harry on 9/13/2023.
//

#ifndef XLUA_DISASSEMBLYUTILITIES_H
#define XLUA_DISASSEMBLYUTILITIES_H
#include "InstructionSet.h"
#include <core/state.h>

namespace LXX
{

class ByteCodeChunk;
namespace Disassembly
{

using namespace LXX::Instruction;

void PrintInstruction( ByteCodeChunk* chunk
                       ,Opcode opcode
                       , OperandType operandType1 , u32 operandIndex1
                       , OperandType operandType2 , u32 operandIndex2
                       , OperandType operandType3 , u32 operandIndex3
                       );


}


}

#endif //XLUA_DISASSEMBLYUTILITIES_H
