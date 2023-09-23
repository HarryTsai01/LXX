//
// Created by Harry on 9/2/2023.
//

#ifndef XLUA_INSTRUCTIONSET_H
#define XLUA_INSTRUCTIONSET_H
#include <core/LXX.h>
#include <core/Containers/Array.h>

namespace LXX
{
/*
 *
 *  an instruction consists of an opcode and two operands
 *  | opcode | operand1 | operand2 | operand3 |
 *  | ------ | -------- | -------- | -------- |
 *  | 6 bit | 18 bit | 18 bit |  18 bit |
 *
 *  an operand consists of a operand type and a index
 *  | operand type | index |
 *   | ----------- | ----- |
 *   | 3 bit | 15 bit |
 *
 * */

class State;
class VirtualMachine;

namespace Instruction
{
enum class OperandType
{
    None = -1 ,
    LocalVariable ,
    TempVariable ,
    Stack ,
    Constant,
    UpValue,
    GlobalVariable,
    Immediate,
    Max ,
};
static_assert( u32( OperandType::Max ) <= 8 , "OperandType::Max <= 8" );

enum class AssertType
{
    ValueType,
    Max,
};

enum class SystemFunction
{
    RaiseException,
    Max,
};

enum class SystemException
{
    ExceptionForZeroReturnValueCount,
    ExceptionForStatementLimitExpressionIsNotNumber,
    ExceptionForStatementIncreaseStepIsNotNumberAndIsNotNil,
    ExceptionForStatementIncreaseStepIsEqualZero,
};


enum class Opcode
{
    None ,
    Call,
    Return,
    Jump,
    SystemCall,
    Assert,
    Push,
    Pop,
    NewTable,
    GetField,
    SetField,
    GetVariableArgument,
    GetReturnValueCount,
    Assignment,
    BinaryOpPower ,
    BinaryOpCmpValueType ,
    BinaryOpCmpLessThan ,
    BinaryOpCmpLessThanOrEqual,
    BinaryOpCmpEqual,
    BinaryOpCmpNotEqual,
    BinaryOpCmpGreaterThan ,
    BinaryOpCmpGreaterThanOrEqual,
    BinaryOpAdd ,
    BinaryOpSubtract,
    BinaryOpMultiply,
    BinaryOpDivide,
    BinaryOpIntDivide,
    BinaryOpModulo,
    BinaryOpBitwiseAnd,
    BinaryOpBitwiseOr,
    BinaryOpBitwiseXor,
    BinaryOpShiftLeft,
    BinaryOpShiftRight,
    BinaryOpConcat,
    BinaryOpLogicalAnd,
    BinaryOpLogicalOr,
    UnaryMinus,
    UnaryBitwiseNot,
    UnaryNot,
    UnaryLen,
    Max ,
};
static_assert( u32( Opcode::Max ) <= 64 , "Opcode::Max <= 64" );


constexpr const u32 OpcodeBitCount = 6;
constexpr const u32 OpcodeBitMask = (1 << OpcodeBitCount) - 1;
constexpr const u32 OperandTypeBitCount = 3;
constexpr const u32 OperandTypeBitMask = (1 << OperandTypeBitCount) - 1;
constexpr const u32 OperandIndexBitCount = 15;
constexpr const u32 OperandIndexBitMask = (1 << OperandIndexBitCount) - 1;
constexpr const u32 OperandBitCount = OperandTypeBitCount + OperandIndexBitCount;
constexpr const u32 OperandBitMask = (1 << OperandBitCount) - 1;

struct InstructionValue
{
    struct Operand
    {
        OperandType _type;
        u32 _index;
    };
    Opcode _opcode;
    Operand _operand1;
    Operand _operand2;
    Operand _operand3;
};

} // Instruction
} // LXX

#endif //XLUA_INSTRUCTIONSET_H
