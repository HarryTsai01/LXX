//
// Created by Harry on 9/2/2023.
//

#include <core/vm/VirtualMachine.h>
#include <core/vm/Instructions/InstructionDecoder.h>
#include <core/objects/metatable/MetaTableMgr.h>

namespace LXX
{


namespace Instruction
{
namespace Decoder
{




} // namespace Decoder
} // namespace Instruction

using namespace Instruction;


Value* VirtualMachine::FindImmediateValue( s32 valueId )
{
    auto it = _immediateValues.Find( valueId );
    if( it == _immediateValues.End() )
    {
        return _immediateValues.Add( valueId , Value( valueId ) );
    }
    return &it->Second;
}


void VirtualMachine::Decode( u64 code , InstructionExecuteContext &context )
{
    auto _decodeOperand = [&]( InstructionValue::Operand& operand )-> Value*
    {
        State* state = context._state;
        ByteCodeChunk* chunk = context._chunk;
        CallInfo * callInfo = state->GetCurrentCallInfo();
        Stack &stack = state->GetStack();
        if( operand._type == OperandType::LocalVariable )
        {
            u32 stackIndex = callInfo->LocalVariableIndexToStackIndex( operand._index );
            return stack.IndexToValue( stackIndex );
        }
        else if( operand._type == OperandType::TempVariable )
        {
            u32 stackIndex = callInfo->TemporaryVariableIndexToStackIndex( operand._index );
            return stack.IndexToValue( stackIndex );
        }
        else if( operand._type == OperandType::Stack )
        {
            return stack.IndexToValue( operand._index );
        }
        else if( operand._type == OperandType::Constant )
        {
            if( !context._chunk->ConstValueIdxIsValid( operand._index ) )
                ThrowError("invalid constant index" );

            return context._chunk->GetConstValue( operand._index );
        }
        else if( operand._type == OperandType::UpValue )
        {
            ThrowError("not implemented");
        }
        else if( operand._type == OperandType::GlobalVariable )
        {
            ThrowError("not implemented");
        }
        else if( operand._type == OperandType::Immediate )
        {
            u32 operandIndex = operand._index & OperandIndexBitMask;
            constexpr u32 operandIndexSignBitMask = 1 <<  ( OperandIndexBitCount - 1 );
            constexpr u32 operandIndexBitMax = (~operandIndexSignBitMask ) & OperandIndexBitMask;

            s32 signedOperandIndex = operandIndex & OperandIndexBitMask;
            if( operandIndex & operandIndexSignBitMask )
            {
                signedOperandIndex = -signedOperandIndex;
            }

            return FindImmediateValue( signedOperandIndex );
        }
        else
        {
            ThrowError("not implemented");
        }
        return nullptr;
    };

    InstructionValue instructionValue ;
    Decoder::Decode( code , instructionValue );

    context._opcode = instructionValue._opcode;
    context._destOperand = _decodeOperand( instructionValue._operand1 );
    context._srcOperand1 = _decodeOperand( instructionValue._operand2 );
    context._srcOperand2 = _decodeOperand( instructionValue._operand3 );
}

void VirtualMachine::RegisterInstructionSet()
{
    _instructionExecuteFunctions.AddZero( static_cast< u32 >( Opcode::Max ) );

    auto _registerInstructionExecuteFunction = [this]( Opcode opcode , InstructionExecuteFunction function )
    {
        _instructionExecuteFunctions[ static_cast< u32 >( opcode ) ] = function;
    };

    _registerInstructionExecuteFunction(Opcode::Call , &VirtualMachine::InstructionExecute_OpcodeCall );
    _registerInstructionExecuteFunction(Opcode::Return , &VirtualMachine::InstructionExecute_OpcodeReturn );
    _registerInstructionExecuteFunction(Opcode::Jump , &VirtualMachine::InstructionExecute_OpcodeJump );
    _registerInstructionExecuteFunction(Opcode::SystemCall , &VirtualMachine::InstructionExecute_OpcodeSystemCall );
    _registerInstructionExecuteFunction(Opcode::Assert , &VirtualMachine::InstructionExecute_OpcodeAssert );
    _registerInstructionExecuteFunction(Opcode::Push , &VirtualMachine::InstructionExecute_OpcodePush );
    _registerInstructionExecuteFunction(Opcode::Pop , &VirtualMachine::InstructionExecute_OpcodePop );
    _registerInstructionExecuteFunction(Opcode::NewTable , &VirtualMachine::InstructionExecute_OpcodeNewTable );
    _registerInstructionExecuteFunction(Opcode::GetField , &VirtualMachine::InstructionExecute_OpcodeGetField );
    _registerInstructionExecuteFunction(Opcode::SetField , &VirtualMachine::InstructionExecute_OpcodeSetField );
    _registerInstructionExecuteFunction(Opcode::GetVariableArgument , &VirtualMachine::InstructionExecute_OpcodeGetVariableArgument );
    _registerInstructionExecuteFunction(Opcode::Assignment , &VirtualMachine::InstructionExecute_Assignment );
    _registerInstructionExecuteFunction(Opcode::BinaryOpPower , &VirtualMachine::InstructionExecute_OpcodeBinaryOpPower );
    _registerInstructionExecuteFunction(Opcode::BinaryOpCmpValueType , &VirtualMachine::InstructionExecute_OpcodeBinaryOpCmpValueType );
    _registerInstructionExecuteFunction(Opcode::BinaryOpCmpLessThan , &VirtualMachine::InstructionExecute_OpcodeBinaryOpCmpLessThan );
    _registerInstructionExecuteFunction(Opcode::BinaryOpCmpLessThanOrEqual , &VirtualMachine::InstructionExecute_OpcodeBinaryOpCmpLessThanOrEqual );
    _registerInstructionExecuteFunction(Opcode::BinaryOpCmpEqual , &VirtualMachine::InstructionExecute_OpcodeBinaryOpCmpEqual );
    _registerInstructionExecuteFunction(Opcode::BinaryOpCmpNotEqual , &VirtualMachine::InstructionExecute_OpcodeBinaryOpCmpNotEqual );
    _registerInstructionExecuteFunction(Opcode::BinaryOpCmpGreaterThan ,&VirtualMachine::InstructionExecute_OpcodeBinaryOpCmpGreaterThan );
    _registerInstructionExecuteFunction(Opcode::BinaryOpCmpGreaterThanOrEqual ,&VirtualMachine::InstructionExecute_OpcodeBinaryOpCmpGreaterThanOrEqual );
    _registerInstructionExecuteFunction(Opcode::BinaryOpAdd , &VirtualMachine::InstructionExecute_OpcodeBinaryOpAdd );
    _registerInstructionExecuteFunction(Opcode::BinaryOpSubtract , &VirtualMachine::InstructionExecute_OpcodeBinaryOpSubtract );
    _registerInstructionExecuteFunction(Opcode::BinaryOpMultiply , &VirtualMachine::InstructionExecute_OpcodeBinaryOpMultiply );
    _registerInstructionExecuteFunction(Opcode::BinaryOpDivide , &VirtualMachine::InstructionExecute_OpcodeBinaryOpDivide );
    _registerInstructionExecuteFunction(Opcode::BinaryOpIntDivide , &VirtualMachine::InstructionExecute_OpcodeBinaryOpIntDivide );
    _registerInstructionExecuteFunction(Opcode::BinaryOpModulo , &VirtualMachine::InstructionExecute_OpcodeBinaryOpModulo );
    _registerInstructionExecuteFunction(Opcode::BinaryOpBitwiseAnd , &VirtualMachine::InstructionExecute_OpcodeBinaryOpBitwiseAnd );
    _registerInstructionExecuteFunction(Opcode::BinaryOpBitwiseOr , &VirtualMachine::InstructionExecute_OpcodeBinaryOpBitwiseOr );
    _registerInstructionExecuteFunction(Opcode::BinaryOpBitwiseXor , &VirtualMachine::InstructionExecute_OpcodeBinaryOpBitwiseXor );
    _registerInstructionExecuteFunction(Opcode::BinaryOpShiftLeft , &VirtualMachine::InstructionExecute_OpcodeBinaryOpShiftLeft );
    _registerInstructionExecuteFunction(Opcode::BinaryOpShiftRight , &VirtualMachine::InstructionExecute_OpcodeBinaryOpShiftRight );
    _registerInstructionExecuteFunction(Opcode::BinaryOpConcat , &VirtualMachine::InstructionExecute_OpcodeBinaryOpConcat );
    _registerInstructionExecuteFunction(Opcode::BinaryOpLogicalAnd , &VirtualMachine::InstructionExecute_OpcodeBinaryOpLogicalAnd );
    _registerInstructionExecuteFunction(Opcode::BinaryOpLogicalOr , &VirtualMachine::InstructionExecute_OpcodeBinaryOpLogicalOr );
    _registerInstructionExecuteFunction(Opcode::UnaryMinus , &VirtualMachine::InstructionExecute_OpcodeUnaryMinus );
    _registerInstructionExecuteFunction(Opcode::UnaryBitwiseNot , &VirtualMachine::InstructionExecute_OpcodeUnaryBitwiseNot );
    _registerInstructionExecuteFunction(Opcode::UnaryNot , &VirtualMachine::InstructionExecute_OpcodeUnaryNot );
    _registerInstructionExecuteFunction(Opcode::UnaryLen , &VirtualMachine::InstructionExecute_OpcodeUnaryLen );

}

#define BEGIN_INSTRUCTION_EXECUTE \
    State *state = context._state; \
    CallInfo *callInfo = state->GetCurrentCallInfo(); \
    Value *destOperand = context._destOperand; \
    Value *srcOperand1 = context._srcOperand1; \
    Value *srcOperand2 = context._srcOperand2;

void VirtualMachine::InstructionExecute_OpcodeCall( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    /*
     * this instruction is used to call a function
     * the first source operand is the argument count
     * the second source operand is the destination operand used to store the return value count
     * the third source operand is none
     * */

    if( !srcOperand1->IsNumber() )
        ThrowError("function call with non-number argument count" );

    s32 argumentValueCount = srcOperand1->As<s32>();
    if( argumentValueCount < 0 )
        ThrowError("function call with negative argument count:%d", argumentValueCount );
    if( destOperand == nullptr )
        ThrowError( " the return value count var is not specified" );

    s32 functionIdx =  - argumentValueCount - 1 ;
    Value *function = state->GetStack().IndexToValue( functionIdx );
    if( function == nullptr )
        ThrowError( " invalid call instruction, function is null" );
    if( !function->IsFunction() )
    {
        ProtectCall( state, argumentValueCount );
        destOperand->Set(callInfo->GetActualReturnValueNum() );
        return;
    }
    // the `function` is not a function
    MetaMethodHandler *metaMethodHandler = MetaTableMgr::GetInstance().GetMetaMethodHandler( function->GetType() );
    if( metaMethodHandler == nullptr )
        ThrowError( "invalid call instruction, meta method handler is null, function type:%d", function->GetType() );

    if( !metaMethodHandler->Invoke( MetaMethodHandler::META_METHOD_KEY_CALL , destOperand , srcOperand1 , srcOperand2  ) )
        ThrowError( "invalid call instruction, meta method handler invoke failed" );
}


void VirtualMachine::InstructionExecute_OpcodeReturn( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    /*
     * the first source operand is the return value count
     * the second source operand is none
     * the third source operand is none
     * */
    if( !srcOperand1->IsNumber() )
        ThrowError("invalid return opcode with non-number return value count" );
    s32 returnValueCount = srcOperand1->As<s32>();
    if( returnValueCount < 0 )
        ThrowError(" invalid return opcode with negative return value count:%d", returnValueCount );

    state->SetLastFunctionCallReturnValueCount(returnValueCount );
    state->GetCurrentCallInfo()->SetActualReturnValueNum(returnValueCount);
}


void VirtualMachine::InstructionExecute_OpcodeJump( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    /*
     *   the first source operand is the jump condition
     *   the second source operand is the jump offset
     *   the third source operand is none
     * */
    if( !srcOperand1->IsBoolean() )
        ThrowError("invalid jump opcode with non-boolean jump condition" );
    if( !srcOperand2->IsNumber() )
        ThrowError("invalid jump opcode with non-number jump offset" );
    if( srcOperand1->As<bool>() )
    {
        if( !callInfo->JumpOffset( srcOperand2->As<s32>() ) )
            ThrowError("invalid jump opcode offset:%d , out of range", srcOperand2->As<s32>());
    }
}


void VirtualMachine::InstructionExecute_OpcodeSystemCall( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    /*
     *  the first operand is none
     *  the second operand is SystemFunction
     *  if the system function is raise exception
     *      then the third operand is SystemException
     * */
    if( !srcOperand1->IsNumber() )
        ThrowError("invalid system call opcode with non-system function" );
    SystemFunction systemFunction = srcOperand1->As<SystemFunction>();
    if( systemFunction >= SystemFunction::Max )
        ThrowError("invalid system call opcode with system function:%d", systemFunction );

    if( systemFunction == SystemFunction::RaiseException )
    {
        if( !srcOperand2->IsNumber() )
            ThrowError("invalid system raise exception opcode with non-system exception" );
        SystemException systemException = srcOperand2->As<SystemException>();
    }
}


void VirtualMachine::InstructionExecute_OpcodeAssert( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    /*
     * the first operand is assert type
     */
    if( !srcOperand1->IsNumber() )
        ThrowError("invalid assert opcode with non-assert type" );
    AssertType assertType = srcOperand1->As<AssertType>();
    if( assertType >= AssertType::Max )
        ThrowError("invalid assert opcode with assert type:%d", assertType );

    if( assertType == AssertType::ValueType )
    {
        /*
         * the second operand is the target operand
         * the third operand is value type
         * */
        if( srcOperand1 == nullptr )
            ThrowError("invalid assert opcode with null target operand" );
        if( srcOperand2 == nullptr )
            ThrowError("invalid assert opcode with null value type" );
        if( !srcOperand2->IsNumber() )
            ThrowError("invalid assert opcode with non-value type" );
        ValueType valueType = srcOperand2->As<ValueType>();

        if( srcOperand2->GetType() != valueType )
            ThrowError("assert failed , the target operand type :%d is not equal to the value type:%d", srcOperand2->GetType(), valueType );
    }
}


void VirtualMachine::InstructionExecute_OpcodePush( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    /*
     * the first source operand is none
     * the second source operand is the value to push into stack
     * the third source operand is none
     * */
    if( srcOperand1 == nullptr )
        ThrowError("invalid push opcode with null source operand" );

    state->GetStack().Push( srcOperand1 );
}


void VirtualMachine::InstructionExecute_OpcodePop( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    /*
     * the first source operand is none
     * the second source operand is the number of values to pop from stack
     * the third source operand is none
     * */
    if( srcOperand1 == nullptr )
        ThrowError("invalid pop opcode with null source operand" );
    if( !srcOperand1->IsNumber() )
        ThrowError("invalid pop opcode with non-number pop count" );

    s32 popCount = srcOperand1->As<s32>();
    if( popCount <= 0 )
        ThrowError("invalid pop opcode with negative pop count or zero pop count, count :%d", popCount );

    state->GetStack().Pop( popCount );
}


void VirtualMachine::InstructionExecute_OpcodeNewTable( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    /*
     * the first source operand is destination operand to store the created table
     * the second source operand is none
     * the third source operand is none
     * */
    if( destOperand == nullptr )
        ThrowError("invalid new table opcode with null destination operand" );

    destOperand->Set( new Table() );
}


void VirtualMachine::InstructionExecute_OpcodeGetField( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    /*
     * this instruction is used to get a field value from a table
     * the first source operand is the destination operand to store the found value
     * the second source operand is the table to find the value
     * the third source operand is the field name
     * */
    if( destOperand == nullptr )
        ThrowError("invalid get field opcode with null destination operand" );
    if( srcOperand1 == nullptr )
        ThrowError("invalid get field opcode with null table operand or null full userdata" );
    if( srcOperand2 == nullptr )
        ThrowError("invalid get field opcode with null field name" );
    if( srcOperand1->GetType() != ValueType::Table
    &&  srcOperand1->GetType() != ValueType::FullUserdata )
        ThrowError("invalid get field opcode with invalid table operand or full userdata" );

    if( srcOperand1->GetType() == ValueType::Table )
    {
        Table *table = srcOperand1->As<Table*>();
        Value* value = table->GetField( this , *srcOperand2 );
        destOperand->Set( value );
    }
}


void VirtualMachine::InstructionExecute_OpcodeSetField( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    /*
     * this instruction is used to set a field value to a table
     * the first source operand is the table to set the value
     * the second source operand is the field value
     * the third source operand is the value to set
     * */
    if( destOperand == nullptr )
        ThrowError("invalid set field opcode with null table operand or null full userdata" );
    if( srcOperand1 == nullptr )
        ThrowError("invalid set field opcode with null field value" );

    if( !destOperand->IsTable() )
        ThrowError("invalid set field opcode with non-table operand , type :%d", destOperand->GetType() );

    Table *table = destOperand->As<Table*>();
    table->SetField( this , *srcOperand1 , *srcOperand2 );
}


void VirtualMachine::InstructionExecute_OpcodeGetVariableArgument( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    /*
     * the first source operand is used to store the total count of the variable argument
     * the second source operand is used to store the start index of the variable argument in the stack
     * the third source operand is used to store the end index of the variable argument in the stack
     * */
    if( destOperand == nullptr )
        ThrowError("invalid get variable argument opcode , the first operand is null" );
    if( srcOperand1 == nullptr )
        ThrowError("invalid get variable argument opcode , the second source operand is null" );
    if( srcOperand2 == nullptr )
        ThrowError("invalid get variable argument opcode , the third source operand is null" );

    u32 variableArgumentStartIndex = -1;
    u32 variableArgumentEndIndex = -1;
    u32 variableArgumentNum = callInfo->GetVariableArgument( variableArgumentStartIndex , variableArgumentEndIndex );

    destOperand->SetAsInt( variableArgumentNum );
    srcOperand1->SetAsInt( variableArgumentStartIndex );
    srcOperand2->SetAsInt( variableArgumentEndIndex );
}


void VirtualMachine::InstructionExecute_Assignment( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    /*
     * the first source operand is the destination operand
     * the second source operand is the source operand
     * the third source operand is none
     * */
    if( destOperand == nullptr )
        ThrowError("invalid assignment opcode with null destination operand" );
    if( srcOperand1 == nullptr )
        ThrowError("invalid assignment opcode with null source operand" );

    destOperand->Set( srcOperand1 );
}


void VirtualMachine::InstructionExecute_OpcodeBinaryOpPower( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    /*
     * the first source operand is the destination operand
     * the second source operand is the base operand
     * the third source operand is the exponent operand
     * */
    InstructionExecute_MetaMethodCall( context , MetaMethodHandler::META_METHOD_KEY_BIN_OP_POWER );
}


void VirtualMachine::InstructionExecute_OpcodeBinaryOpCmpValueType( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    /*
     * this instruction is used to test whether the first source operand is the same as the second source operand specified
     * the first source operand is the destination operand used to store the test result
     * the second source operand is immediate value to specific the value type
     * */
    if( destOperand == nullptr )
        ThrowError("invalid compare value type opcode with null destination operand" );
    if( srcOperand1 == nullptr )
        ThrowError("invalid compare value type opcode with null source operand" );
    if( srcOperand2 == nullptr )
        ThrowError("invalid compare value type opcode with null value type operand" );
    if( !srcOperand2->IsNumber()
        || srcOperand2->As<ValueType>() < ValueType::Start
        || srcOperand2->As<ValueType>() > ValueType::Max
        )
        ThrowError("invalid compare value type opcode with invalid value type operand" );

    destOperand->Set(  srcOperand1->GetType() == srcOperand2->As<ValueType>() );
}


void VirtualMachine::InstructionExecute_OpcodeBinaryOpCmpLessThan( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    InstructionExecute_MetaMethodCall( context , MetaMethodHandler::META_METHOD_KEY_BIN_OP_LESS_THAN );
}


void VirtualMachine::InstructionExecute_OpcodeBinaryOpCmpLessThanOrEqual( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    InstructionExecute_MetaMethodCall( context , MetaMethodHandler::META_METHOD_KEY_BIN_OP_LESS_THAN_OR_EQUAL );
}


void VirtualMachine::InstructionExecute_OpcodeBinaryOpCmpEqual( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;

    InstructionExecute_MetaMethodCall( context , MetaMethodHandler::META_METHOD_KEY_BIN_OP_EQUAL );
}


void VirtualMachine::InstructionExecute_OpcodeBinaryOpCmpNotEqual( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    InstructionExecute_MetaMethodCall( context , MetaMethodHandler::META_METHOD_KEY_BIN_OP_EQUAL );
    destOperand->Set( !destOperand->As<bool>() );
}


void VirtualMachine::InstructionExecute_OpcodeBinaryOpCmpGreaterThan( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    InstructionExecute_MetaMethodCall( context , MetaMethodHandler::META_METHOD_KEY_BIN_OP_LESS_THAN_OR_EQUAL );
    destOperand->Set( !destOperand->As<bool>() );
}


void VirtualMachine::InstructionExecute_OpcodeBinaryOpCmpGreaterThanOrEqual( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    InstructionExecute_MetaMethodCall( context , MetaMethodHandler::META_METHOD_KEY_BIN_OP_LESS_THAN );
    destOperand->Set( !destOperand->As<bool>() );
}


void VirtualMachine::InstructionExecute_OpcodeBinaryOpAdd( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    InstructionExecute_MetaMethodCall( context , MetaMethodHandler::META_METHOD_KEY_BIN_OP_ADD );
    return;
    if( context._srcOperand1->IsNumber() && context._srcOperand2->IsNumber() )
    {
        f64 src1 = context._srcOperand1->AsReal();
        f64 src2 = context._srcOperand2->AsReal();
        f64 result = src1 + src2;

        if( context._srcOperand1->IsReal() || context._srcOperand2->IsReal() )
        {
            context._destOperand->SetAsReal( result );
        }
        else
        {
            context._destOperand->SetAsInt( result );
        }
    }
    else
    {
        ThrowError("non-legal operand for add");
    }
}


void VirtualMachine::InstructionExecute_OpcodeBinaryOpSubtract( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    InstructionExecute_MetaMethodCall( context , MetaMethodHandler::META_METHOD_KEY_BIN_OP_SUB );
}


void VirtualMachine::InstructionExecute_OpcodeBinaryOpMultiply( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    InstructionExecute_MetaMethodCall( context , MetaMethodHandler::META_METHOD_KEY_BIN_OP_MUL );

}


void VirtualMachine::InstructionExecute_OpcodeBinaryOpDivide( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    InstructionExecute_MetaMethodCall( context , MetaMethodHandler::META_METHOD_KEY_BIN_OP_DIV );
}


void VirtualMachine::InstructionExecute_OpcodeBinaryOpIntDivide( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    InstructionExecute_MetaMethodCall( context , MetaMethodHandler::META_METHOD_KEY_BIN_OP_IDIV );
}


void VirtualMachine::InstructionExecute_OpcodeBinaryOpModulo( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    InstructionExecute_MetaMethodCall( context , MetaMethodHandler::META_METHOD_KEY_BIN_OP_MOD );
}


void VirtualMachine::InstructionExecute_OpcodeBinaryOpBitwiseAnd( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    InstructionExecute_MetaMethodCall( context , MetaMethodHandler::META_METHOD_KEY_BIN_OP_BITWISE_AND );
}


void VirtualMachine::InstructionExecute_OpcodeBinaryOpBitwiseOr( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    InstructionExecute_MetaMethodCall( context , MetaMethodHandler::META_METHOD_KEY_BIN_OP_BITWISE_OR );
}


void VirtualMachine::InstructionExecute_OpcodeBinaryOpBitwiseXor( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    InstructionExecute_MetaMethodCall( context , MetaMethodHandler::META_METHOD_KEY_BIN_OP_BITWISE_XOR );
}


void VirtualMachine::InstructionExecute_OpcodeBinaryOpShiftLeft( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    InstructionExecute_MetaMethodCall( context , MetaMethodHandler::META_METHOD_KEY_BIN_OP_SHIFT_LEFT );
}


void VirtualMachine::InstructionExecute_OpcodeBinaryOpShiftRight( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    InstructionExecute_MetaMethodCall( context , MetaMethodHandler::META_METHOD_KEY_BIN_OP_SHIFT_RIGHT );
}


void VirtualMachine::InstructionExecute_OpcodeBinaryOpConcat( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    InstructionExecute_MetaMethodCall( context , MetaMethodHandler::META_METHOD_KEY_BIN_OP_CONCAT );
}


void VirtualMachine::InstructionExecute_OpcodeBinaryOpLogicalAnd( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    /*
     * this instruction is make a logical and between two operands ,
     * the first operand is the result of the logical and ,
     * the second operand is boolean operand
     * the third operand is boolean operand
     * */
    if( destOperand == nullptr )
        ThrowError("invalid logical and opcode with null destination operand");
    if( srcOperand1 == nullptr || !srcOperand1->IsBoolean() )
        ThrowError("invalid logical and opcode with null or non-boolean source operand1");
    if( srcOperand2 == nullptr || !srcOperand2->IsBoolean() )
        ThrowError("invalid logical and opcode with null or non-boolean source operand2");

    destOperand->Set( srcOperand1->As<bool>() && srcOperand2->As<bool>() );
}


void VirtualMachine::InstructionExecute_OpcodeBinaryOpLogicalOr( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    /*
     * this instruction is make a logical or between two operands ,
     * the first operand is the result of the logical or ,
     * the second operand is boolean operand
     * the third operand is boolean operand
     * */
    if( destOperand == nullptr )
        ThrowError("invalid logical or opcode with null destination operand");
    if( srcOperand1 == nullptr || !srcOperand1->IsBoolean() )
        ThrowError("invalid logical or opcode with null or non-boolean source operand1");
    if( srcOperand2 == nullptr || !srcOperand2->IsBoolean() )
        ThrowError("invalid logical or opcode with null or non-boolean source operand2");

    destOperand->Set( srcOperand1->As<bool>() || srcOperand2->As<bool>() );

}


void VirtualMachine::InstructionExecute_OpcodeUnaryMinus( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    InstructionExecute_MetaMethodCall( context , MetaMethodHandler::META_METHOD_KEY_UNA_OP_MINUS );
}


void VirtualMachine::InstructionExecute_OpcodeUnaryBitwiseNot( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    InstructionExecute_MetaMethodCall( context , MetaMethodHandler::META_METHOD_KEY_UNA_OP_BITWISE_NOT );
}


void VirtualMachine::InstructionExecute_OpcodeUnaryNot( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    /*
     * this instruction is make a logical not operation with the first source operand
     * the first operand is the result of the logical not ,
     * the second operand is boolean operand
     * the third operand is ignored
     * */
    if( destOperand == nullptr )
        ThrowError("invalid logical not opcode with null destination operand");
    if( srcOperand1 == nullptr || !srcOperand1->IsBoolean() )
        ThrowError("invalid logical not opcode with null or non-boolean source operand1");

    destOperand->Set( !srcOperand1->As<bool>() );
}


void VirtualMachine::InstructionExecute_OpcodeUnaryLen( InstructionExecuteContext &context )
{
    BEGIN_INSTRUCTION_EXECUTE;
    InstructionExecute_MetaMethodCall( context , MetaMethodHandler::META_METHOD_KEY_UNA_OP_LENGTH );
}

void VirtualMachine::InstructionExecute_MetaMethodCall( InstructionExecuteContext &context , const char * methodKey )
{
    BEGIN_INSTRUCTION_EXECUTE;
    if( destOperand == nullptr )
        ThrowError("invalid meta method call opcode with null destination operand , methodKey:%s" , methodKey );
    if( srcOperand1 == nullptr )
        ThrowError("invalid meta method call opcode with null source operand1 , methodKey:%s" , methodKey );
    if( srcOperand2 == nullptr )
        ThrowError("invalid meta method call opcode with null source operand2 , methodKey:%s" , methodKey );

    if( srcOperand1->IsNumber()
    && srcOperand2->IsNumber() )
    {
        MetaMethodHandler *handler = MetaTableMgr::GetInstance().GetMetaMethodHandler( ValueType::Number );
        if( handler == nullptr )
            ThrowError("invalid meta method call opcode can't find meta method handler , methodKey:%s" , methodKey );
        if( !handler->Invoke(methodKey, context._destOperand, context._srcOperand1, context._srcOperand2) )
            ThrowError(" failed to invoke binary operator in meta method handler , methodKey:%s , ValueType:%d" , methodKey , ValueType::Number );
    }
    else
    {
        /*
         * the number of the meta method handler would only process the situation that
         *   both the two operands are numbers , otherwise would raise an exception.
         *   so we would only check the meta method handler for the first operand when it is not a number.
         * */
        MetaMethodHandler * handler = nullptr;
        if( !srcOperand1->IsNumber() )
            handler = MetaTableMgr::GetInstance().GetMetaMethodHandler( context._srcOperand1->GetType() );

        if( handler == nullptr )
            handler = MetaTableMgr::GetInstance().GetMetaMethodHandler( context._srcOperand2->GetType() );
        if( handler == nullptr )
            ThrowError("invalid meta method call opcode neither can find meta method handler in the first nor in the second operand, methodKey:%s srcOperand1:%d , srcOperand2:%d"
                       , methodKey
                       , context._srcOperand1->GetType( )
                       , context._srcOperand2->GetType( )
                       );

        if( !handler->Invoke(methodKey, context._destOperand, context._srcOperand1, context._srcOperand2) )
            ThrowError(" failed to invoke binary operator in meta method handler , methodKey:%s , srcOperand1:%d , srcOperand2:%d "
                       , methodKey
                       , context._srcOperand1->GetType( )
                       , context._srcOperand2->GetType( )
                       );
    }
}


u32 VirtualMachine::CallLuaClosure( State *state , LuaClosure *closure )
{
    const Array< u64 > &code = closure->GetChunk()->GetCode();
    InstructionExecuteContext context ( this , state , closure->GetChunk( ) ) ;
    auto FetchInstructionFunctionPtrAndCheck = [&]()-> InstructionExecuteFunction
    {
        u32 opcodeIdx = static_cast< u32 >( context._opcode );
        if( !_instructionExecuteFunctions.IsValidIndex( opcodeIdx ) )
            ThrowError(" Invalid  opcode : %d " , context._opcode );
        auto executeFunction = _instructionExecuteFunctions[ static_cast< u32 >( context._opcode ) ];
        if( executeFunction == nullptr )
            ThrowError("opcode:%d not implemented" , context._opcode );
        return executeFunction;
    };

    u64 instruction;
    while( state->FetchNextInstruction( instruction ))
    {
        Decode(  instruction , context );
        auto executeFunction = FetchInstructionFunctionPtrAndCheck( );
        (this->*executeFunction)( context );
    }

    return state->GetCurrentCallInfo()->GetActualReturnValueNum();
}


LuaClosure* VirtualMachine::CompileFile( String *scriptFileName )
{
    Compiler * compiler = new Compiler( );
    // todo , hold compiler to prevent gc
    return compiler->CompileFile( scriptFileName );
}


LuaClosure* VirtualMachine::CompileString( const char* scriptContent )
{
    Compiler * compiler = new Compiler( );
    // todo , hold compiler to prevent gc
    return compiler->CompileString( scriptContent );
}

}

