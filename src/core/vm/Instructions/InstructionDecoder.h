//
// Created by Harry on 9/2/2023.
//

#ifndef XLUA_INSTRUCTIONDECODER_H
#define XLUA_INSTRUCTIONDECODER_H
#include "InstructionSet.h"

namespace LXX
{
namespace Instruction
{
namespace Decoder
{

void Decode( u64 i , InstructionValue &out );

s32 GetOperandIndex(u64 operand );

} // namespace Decoder
} // namespace InstructionSet
} // namespace LXX



#endif //XLUA_INSTRUCTIONDECODER_H
