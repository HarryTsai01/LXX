//
// Created by Harry on 9/2/2023.
//

#ifndef XLUA_INSTRUCTIONENCODER_H
#define XLUA_INSTRUCTIONENCODER_H
#include <core/LXX.h>
#include <core/Containers/Array.h>
#include <core/vm/Instructions/InstructionSet.h>

namespace LXX
{
class ByteCodeChunk;
class StatementBase;
class LuaClosure;
class CompileContext;
namespace Instruction
{
namespace Encoder
{


u64 MakeOperandIndex( s32 value );


template< typename T >
u64 MakeOperand( OperandType type, T value )
{
    return MakeOperand( type, static_cast< u64 >( value ) );
}

template<>
u64 MakeOperand( OperandType type, u64 value );

u64 MakeInstruction(Opcode opcode, u64 destOperand , u64 srcOperand1 , u64 srcOperand2 );

void Encode( Array< u64 > &code , Opcode opcode , u64 destOperand , u64 srcOperand1 , u64 srcOperand2 );

u64 Encode( Opcode opcode , u64 destOperand , u64 srcOperand1 , u64 srcOperand2 );


class Helper
{
public:
    Helper( CompileContext *context );

    void Jump( u64 condition , u64 jumpInstructionLocation , u64 destinationLocation );
    void Jump( u64 condition , u64 destinationLocation );
    void JumpAlways( u64 destinationLocation );
    void JumpAlways( u64 jumpInstructionLocation , u64 destinationLocation );
    void JumpOffset( s32 offset );
    void JumpOffset( u64 condition , s32 offset );

    void Push( OperandType operandType , u64 operandIdx );
    void Pop( OperandType operandType , u64 operandIdx );
    void Return( u64 returnValueCount );
    void Return( OperandType returnCountType , u64 returnCountIdx );

    void Assign( OperandType destinationType , u64 destinationIdx , OperandType sourceType , u64 sourceIdx );
    void SetField( OperandType tableType , u64 tableIdx , OperandType fieldType , u64 fieldIdx , OperandType valueType , u64 valueIdx );
    void GetField( OperandType destinationType , u64 destinationIdx ,OperandType tableType , u64 tableIdx , OperandType fieldType , u64 fieldIdx );
    void EncodeDirectly( Opcode opcode ,
                                 OperandType destOperand , u64 destOperandIdx ,
                                 OperandType srcOperand1 , u64 srcOperandIdx1 ,
                                 OperandType srcOperand2 , u64 srcOperandIdx2
    );
    void NewTable( OperandType destinationType , u64 destinationIdx );
    void Call( u32 argumentCount , OperandType returnType , u64 returnIdx );
    void Call( OperandType argumentCountType , u64 argumentCountIdx , OperandType returnType , u64 returnIdx );
    void UnaryNot( OperandType destinationType , u64 destinationIdx , OperandType sourceType , u64 sourceIdx );
    void AssertValueType( OperandType operandType , u64 operandIdx ,  ValueType valueType );
    void CompareValueType( OperandType resultType , u64 resultIdx , OperandType operandType , u64 operandIdx ,  ValueType valueType );
    void RaiseException( SystemException exceptionIdx );
    void Increase( OperandType operandType , u64 operandIdx ,  s32 amount );
    void GetVariableArgument( OperandType startIndexType , u64 startIndexIdx , OperandType  endIndexType , u64 endIndexIdx ,  OperandType argumentCountType , u64 argumentCountIdx );
    u32 AddPlaceholder();

    u32 GetNextInstructionLocation() const { return _bytecode.Size(); }

    bool IsPlaceHolder( u32 index );

    template< typename EncodeConditionFunction , typename EncodeTrueBranchFunction , typename EncodeFalseBranchFunction >
    void EncodeSimpleIfStatement( EncodeConditionFunction encodeCondition,
                                  OperandType conditionResultType , u64 conditionResultIdx ,
                                  EncodeTrueBranchFunction encodeTrueBranch ,
                                  EncodeFalseBranchFunction encodeFalseBranch
    )
    {
        encodeCondition( *this );
        u32 jumpToTrueBranchPlaceholder = AddPlaceholder();

        encodeFalseBranch( *this );
        u32 jumpToEndPlaceholder = AddPlaceholder();

        u32 trueBranchStartLocation = _bytecode.Size();
        encodeTrueBranch( *this );

        Jump( Encoder::MakeOperand( conditionResultType , conditionResultIdx ) ,
              jumpToTrueBranchPlaceholder ,
              trueBranchStartLocation
        );

        JumpAlways(
                jumpToEndPlaceholder ,
                _bytecode.Size()
        );
    }

    template< typename EncodeConditionFunction , typename EncodeTrueBranchFunction >
    void EncodeSimpleIfStatement( EncodeConditionFunction encodeCondition,
                                  OperandType conditionResultType , u64 conditionResultIdx ,
                                  EncodeTrueBranchFunction encodeTrueBranch
    )
    {
        encodeCondition( *this );
        UnaryNot( OperandType::TempVariable , conditionResultIdx ,
                  OperandType::TempVariable , conditionResultIdx );
        u32 jumpToEndPlaceholder = AddPlaceholder();
        encodeTrueBranch( *this );

        Jump( Encoder::MakeOperand( conditionResultType , conditionResultIdx ) ,
              jumpToEndPlaceholder ,
              GetNextInstructionLocation()
        );
    }

    template< typename EncodeConditionFunction , typename EncodeLoopBodyFunction >
    void EncodeWhileStatement(EncodeConditionFunction encodeCondition,
                              OperandType conditionResultType , u64 conditionResultIdx ,
                              EncodeLoopBodyFunction encodeLoopBody
    )
    {
        u32 loopStartLocation = GetNextInstructionLocation();
        encodeCondition( *this );
        u32 exitLoopPlaceholder = AddPlaceholder();
        encodeLoopBody( *this );
        JumpAlways( loopStartLocation );
        u32 loopEndLocation = GetNextInstructionLocation();
        Jump( Encoder::MakeOperand( conditionResultType , conditionResultIdx ) ,
              exitLoopPlaceholder ,
              loopEndLocation
              );

        ProcessBreakAndContinueStatementInLoopStatement( loopStartLocation , loopEndLocation );
    }

    template< typename EncodeConditionFunction , typename EncodeLoopBodyFunction >
    void EncodeRepeatStatement(EncodeConditionFunction encodeCondition,
                              OperandType conditionResultType , u64 conditionResultIdx ,
                              EncodeLoopBodyFunction encodeLoopBody
    )
    {
        u32 loopStartLocation = GetNextInstructionLocation();
        encodeLoopBody( *this );
        encodeCondition( *this );
        UnaryNot( OperandType::TempVariable , conditionResultIdx ,
                               OperandType::TempVariable , conditionResultIdx );
        Jump( Encoder::MakeOperand( conditionResultType , conditionResultIdx ) ,
              loopStartLocation );
        u32 loopEndLocation = GetNextInstructionLocation();

        ProcessBreakAndContinueStatementInLoopStatement( loopStartLocation , loopEndLocation );
    }

    template< typename EncodeInitializerFunction ,
            typename EncodeConditionFunction ,
            typename EncodeLoopBodyFunction
            >
    void EncodeForLoopStatement(
                              EncodeInitializerFunction encodeInitializer,
                              EncodeConditionFunction encodeCondition,
                              OperandType conditionResultType , u64 conditionResultIdx ,
                              EncodeLoopBodyFunction encodeLoopBody
    )
    {
        encodeInitializer( *this );
        u32 loopStartLocation = GetNextInstructionLocation();
        encodeCondition( *this );
        u32 exitLoopPlaceholder = AddPlaceholder();
        encodeLoopBody( *this );
        JumpAlways( loopStartLocation );
        u32 loopEndLocation = GetNextInstructionLocation();
        Jump( Encoder::MakeOperand( conditionResultType , conditionResultIdx ) ,
              exitLoopPlaceholder ,
              loopEndLocation
        );

        ProcessBreakAndContinueStatementInLoopStatement( loopStartLocation , loopEndLocation );
    }
private:
    void ProcessBreakAndContinueStatementInLoopStatement( u32 loopBeginLocation , u32 loopEndLocation );
private:
    CompileContext *_context;
    Array< u64 > &_bytecode;
};


} // Encoder
} // Instruction
} // LXX

#endif //XLUA_INSTRUCTIONENCODER_H
