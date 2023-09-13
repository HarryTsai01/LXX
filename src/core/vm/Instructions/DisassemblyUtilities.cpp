//
// Created by Harry on 9/13/2023.
//
#include "DisassemblyUtilities.h"
#include "InstructionDecoder.h"
#include <core/containers/UnorderedMap.h>
#include <core/vm/ByteCodeChunk.h>
#include <iostream>

namespace LXX
{
namespace Disassembly
{

static UnorderedMap< Opcode , const char* > _opcodeMaps =
{
        { Opcode::Call , "Call" } ,
        { Opcode::Return , "Return" } ,
        { Opcode::Jump , "Jump" } ,
        { Opcode::SystemCall , "SystemCall" } ,
        { Opcode::Assert  , "Assert" } ,
        { Opcode::Push , "Push" } ,
        { Opcode::Pop , "Pop" } ,
        { Opcode::NewTable , "NewTable" } ,
        { Opcode::GetField , "GetField" } ,
        { Opcode::SetField , "SetField" } ,
        { Opcode::GetVariableArgument , "GetVariableArgument" } ,
        { Opcode::Assignment , "Assignment" } ,
        { Opcode::BinaryOpPower , "BinaryOpPower" } ,
        { Opcode::BinaryOpCmpValueType , "BinaryOpCmpValueType" } ,
        { Opcode::BinaryOpCmpLessThan , "BinaryOpCmpLessThan" } ,
        { Opcode::BinaryOpCmpLessThanOrEqual, "BinaryOpCmpLessThanOrEqual" } ,
        { Opcode::BinaryOpCmpEqual, "BinaryOpCmpEqual" } ,
        { Opcode::BinaryOpCmpNotEqual, "BinaryOpCmpNotEqual" } ,
        { Opcode::BinaryOpCmpGreaterThan , "BinaryOpCmpGreaterThan" } ,
        { Opcode::BinaryOpCmpGreaterThanOrEqual, "BinaryOpCmpGreaterThanOrEqual" } ,
        { Opcode::BinaryOpAdd , "BinaryOpAdd" } ,
        { Opcode::BinaryOpSubtract, "BinaryOpSubtract" } ,
        { Opcode::BinaryOpMultiply, "BinaryOpMultiply" } ,
        { Opcode::BinaryOpDivide, "BinaryOpDivide" } ,
        { Opcode::BinaryOpIntDivide, "BinaryOpIntDivide" } ,
        { Opcode::BinaryOpModulo, "BinaryOpModulo" } ,
        { Opcode::BinaryOpBitwiseAnd, "BinaryOpBitwiseAnd" } ,
        { Opcode::BinaryOpBitwiseOr, "BinaryOpBitwiseOr" } ,
        { Opcode::BinaryOpBitwiseXor, "BinaryOpBitwiseXor" } ,
        { Opcode::BinaryOpShiftLeft, "BinaryOpShiftLeft" } ,
        { Opcode::BinaryOpShiftRight, "BinaryOpShiftRight" } ,
        { Opcode::BinaryOpConcat, "BinaryOpConcat" } ,
        { Opcode::BinaryOpLogicalAnd, "BinaryOpLogicalAnd" } ,
        { Opcode::BinaryOpLogicalOr, "BinaryOpLogicalOr" } ,
        { Opcode::UnaryMinus, "UnaryMinus" } ,
        { Opcode::UnaryBitwiseNot, "UnaryBitwiseNot" } ,
        { Opcode::UnaryNot, "UnaryNot" } ,
        { Opcode::UnaryLen, "UnaryLen" } ,
};
static UnorderedMap< OperandType , const char* > _opcodeTypeMaps =
{
        { OperandType::None , "None" },
        { OperandType::LocalVariable , "LocalVariable" },
        { OperandType::TempVariable , "TempVariable" },
        { OperandType::Stack , "Stack" },
        { OperandType::Constant , "Constant" },
        { OperandType::UpValue , "UpValue" },
        { OperandType::GlobalVariable , "GlobalVariable" },
        { OperandType::Immediate , "Immediate" },
        { OperandType::Max ,  "Max" },
};


void PrintInstruction( ByteCodeChunk* chunk
        , Opcode opcode
        , OperandType operandType1 , u32 operandIndex1
        , OperandType operandType2 , u32 operandIndex2
        , OperandType operandType3 , u32 operandIndex3
)
{
    auto printOperand = [&]( ostream &os, OperandType type , u32 index )
    {
        os << _opcodeTypeMaps[type] << index;
        if( type == OperandType::Constant )
        {
            Value* constValue = chunk->GetConstValue( index );
            if( constValue->IsString() )
                os << "=" << constValue->As<String*>();
            else if( constValue->IsInteger() )
                os << "=" << constValue->As<s32>();
            else if( constValue->IsReal() )
                os << "=" << constValue->As<f64>();
        }
        else if( type == OperandType::Immediate )
        {
            os << "=" << index;
        }
        else if( type == OperandType::Stack )
        {
            os << "=" << Decoder::GetOperandIndex( index );
        }
    };
   std::cout << _opcodeMaps[opcode] << "(";
   printOperand( std::cout , operandType1 , operandIndex1 );
   std::cout << ", ";
   printOperand( std::cout , operandType2 , operandIndex2 );
    std::cout << ", ";
   printOperand( std::cout , operandType3 , operandIndex3 );
   std::cout<< ")" << std::endl;
}

} // namespace Disassembly
} // namespace LXX
