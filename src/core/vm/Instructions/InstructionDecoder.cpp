//
// Created by Harry on 9/2/2023.
//

#include "InstructionDecoder.h"

namespace LXX
{
namespace Instruction
{
namespace Decoder
{

void Decode( u64 i , InstructionValue &out )
{
    out._opcode = static_cast< Opcode >( ( i >> ( OperandBitCount * 3 ) ) & OpcodeBitMask );
    u64 operand1 = ( i >> ( OperandBitCount * 2 ) ) & OperandBitMask;
    u64 operand2 = ( i >> OperandBitCount ) & OperandBitMask;
    u64 operand3 = i & OperandBitMask;

    auto decodeOperand = [&]( u64 operand , InstructionValue::Operand &out )
    {
        out._type = static_cast< OperandType >( ( operand >> OperandIndexBitCount ) & OperandTypeBitMask );
        out._index = operand & OperandIndexBitMask;
    };

    decodeOperand( operand1 , out._operand1 );
    decodeOperand( operand2 , out._operand2 );
    decodeOperand( operand3 , out._operand3 );
}


s32 GetOperandIndex( u64 operand )
{
    u32 signBit = operand & ( 1 << ( OperandIndexBitCount - 1 ) );

    s32 sValue = static_cast< s32 >( operand & ~( 1 << ( OperandIndexBitCount - 1 ) ) );
    return signBit ? - sValue : sValue;
}


} // namespace Decoder
} // namespace InstructionSet
} // namespace LXX
