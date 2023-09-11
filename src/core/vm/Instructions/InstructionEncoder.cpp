//
// Created by Harry on 9/2/2023.
//

#include "InstructionEncoder.h"
#include <core/compiler/Compiler.h>

namespace LXX
{
namespace Instruction
{
namespace Encoder
{

Helper::Helper( CompileContext *context )
    : _context( context )
    , _bytecode(context->GetLuaClosure()->GetChunk()->GetCode() )
{

}


void Helper::Jump( u64 condition , u64 jumpInstructionLocation , u64 destinationLocation )
{
    s32 jumpOffset = destinationLocation - jumpInstructionLocation - 1;
    _bytecode[ jumpInstructionLocation ] =
            Encode( Opcode::Jump ,
                    Encoder::MakeOperand( OperandType::None , 0 ) ,
                    condition ,
                    Encoder::MakeOperand( OperandType::Immediate , Encoder::MakeOperandIndex( jumpOffset ) )
                    );
}


void Helper::Jump( u64 condition , u64 destinationLocation )
{
    _bytecode.PushBack(0 );
    Jump(condition , _bytecode.Size() - 1 , destinationLocation );
}


void Helper::JumpAlways( u64 destinationLocation )
{
    Jump( MakeOperand( OperandType::Immediate , true ) , destinationLocation );
}


void Helper::JumpAlways( u64 jumpInstructionLocation , u64 destinationLocation )
{
    Jump( MakeOperand( OperandType::Immediate , true ) , jumpInstructionLocation , destinationLocation );
}


void Helper::JumpOffset( s32 offset )
{
    JumpAlways(_bytecode.Size() + offset );
}


void Helper::JumpOffset( u64 condition , s32 offset )
{
    Jump(condition , _bytecode.Size() + offset );
}

void Helper::Push( OperandType operandType , u64 operandIdx )
{
    Encoder::Encode(
            _bytecode ,
            Opcode::Push,
            Encoder::MakeOperand( OperandType::None , 0 ),
            Encoder::MakeOperand( operandType , operandIdx ),
            Encoder::MakeOperand( OperandType::None , 0 )
    );
}


void Helper::Pop( OperandType operandType , u64 operandIdx )
{
    Encoder::Encode(
            _bytecode ,
            Opcode::Pop ,
            Encoder::MakeOperand( OperandType::None , 0 ),
            Encoder::MakeOperand( operandType , operandIdx ),
            Encoder::MakeOperand( OperandType::None , 0 )
    );
}


void Helper::Return( u64 returnValueCount )
{
    Return( OperandType::Immediate , returnValueCount );
}


void Helper::Return( OperandType returnCountType , u64 returnCountIdx )
{
    Encoder::Encode(
            _bytecode ,
            Opcode::Return ,
            Encoder::MakeOperand( returnCountType , returnCountIdx ) ,
            Encoder::MakeOperand( OperandType::None , 0 ),
            Encoder::MakeOperand( OperandType::None , 0 )
    );
}


void Helper::Assign( OperandType destinationType , u64 destinationIdx , OperandType sourceType , u64 sourceIdx )
{
    Encoder::Encode(
            _bytecode ,
            Opcode::Assignment ,
            Encoder::MakeOperand( destinationType , destinationIdx ) ,
            Encoder::MakeOperand( sourceType , sourceIdx ) ,
            Encoder::MakeOperand( OperandType::None , 0 )
    );
}


void Helper::SetField( OperandType tableType , u64 tableIdx , OperandType fieldType , u64 fieldIdx , OperandType valueType , u64 valueIdx )
{
    Encoder::Encode(_bytecode,
                    Opcode::SetField ,
                    Encoder::MakeOperand( tableType , tableIdx ) ,
                    Encoder::MakeOperand( fieldType , fieldIdx ) ,
                    Encoder::MakeOperand( valueType , valueIdx )
    );
}


void Helper::GetField( OperandType destinationType , u64 destinationIdx ,OperandType tableType , u64 tableIdx , OperandType fieldType , u64 fieldIdx )
{
    Encoder::Encode(_bytecode,
                    Opcode::GetField ,
                    Encoder::MakeOperand( destinationType , destinationIdx ) ,
                    Encoder::MakeOperand( tableType , tableIdx ) ,
                    Encoder::MakeOperand( fieldType , fieldIdx )
    );
}


void Helper::EncodeDirectly( Opcode opcode ,
                             OperandType destOperand , u64 destOperandIdx ,
                             OperandType srcOperand1 , u64 srcOperandIdx1 ,
                             OperandType srcOperand2 , u64 srcOperandIdx2
                             )
{
    Encoder::Encode(_bytecode ,
                    opcode ,
                    Encoder::MakeOperand( destOperand , destOperandIdx ) ,
                    Encoder::MakeOperand( srcOperand1 , srcOperandIdx1 ) ,
                    Encoder::MakeOperand( srcOperand2 , srcOperandIdx2 )
            );
}


void Helper::NewTable( OperandType destinationType , u64 destinationIdx )
{
    Encoder::Encode(_bytecode,
                    Opcode::NewTable ,
                    Encoder::MakeOperand( destinationType , destinationIdx ) ,
                    Encoder::MakeOperand( OperandType::None , 0 ),
                    Encoder::MakeOperand( OperandType::None , 0 ) );
}


void Helper::Call( u32 argumentCount , OperandType returnType , u64 returnIdx )
{
    Call(  OperandType::Immediate , argumentCount , returnType , returnIdx );
}


void Helper::Call( OperandType argumentCountType , u64 argumentCountIdx , OperandType returnType , u64 returnIdx )
{
    Encoder::Encode(_bytecode,
                    Opcode::Call ,
                    Encoder::MakeOperand( returnType , returnIdx ) ,
                    Encoder::MakeOperand( argumentCountType , argumentCountIdx ) ,
                    Encoder::MakeOperand( OperandType::None , 0 )
    );
}


void Helper::UnaryNot( OperandType destinationType , u64 destinationIdx , OperandType sourceType , u64 sourceIdx )
{
    Encoder::Encode(_bytecode ,
                    Opcode::UnaryNot ,
                    Encoder::MakeOperand( destinationType , destinationIdx ) ,
                    Encoder::MakeOperand( sourceType , sourceIdx ) ,
                    Encoder::MakeOperand( OperandType::None , 0 )
    );
}


void Helper::AssertValueType( OperandType operandType , u64 operandIdx,  ValueType valueType  )
{
    Encoder::Encode(_bytecode ,
                    Opcode::Assert,
                    Encoder::MakeOperand( OperandType::Immediate , AssertType::ValueType ) ,
                    Encoder::MakeOperand( operandType , operandIdx ),
                 Encoder::MakeOperand( OperandType::Immediate , valueType )
    );
}


void Helper::CompareValueType( OperandType resultType , u64 resultIdx , OperandType operandType , u64 operandIdx ,  ValueType valueType )
{
    Encoder::Encode(_bytecode ,
                    Opcode::BinaryOpCmpValueType,
                    Encoder::MakeOperand( resultType , resultIdx ) ,
                    Encoder::MakeOperand( operandType , operandIdx ),
                    Encoder::MakeOperand( OperandType::Immediate , valueType )
    );
}


void Helper::RaiseException( SystemException exceptionIdx )
{
    Encoder::Encode(
            _bytecode ,
            Opcode::SystemCall ,
            Encoder::MakeOperand( OperandType::Immediate, SystemFunction::RaiseException ) ,
            Encoder::MakeOperand( OperandType::Immediate , exceptionIdx ),
            Encoder::MakeOperand( OperandType::Immediate , 0 )
    );
}


void Helper::Increase( OperandType operandType , u64 operandIdx ,  s32 amount )
{
    EncodeDirectly( Opcode::BinaryOpAdd ,
     operandType , operandIdx ,
    operandType , operandIdx ,
     OperandType::Immediate , MakeOperandIndex( amount )
    );
}


void Helper::GetVariableArgument( OperandType startIndexType , u64 startIndexIdx , OperandType  endIndexType , u64 endIndexIdx ,  OperandType argumentCountType , u64 argumentCountIdx )
{
    EncodeDirectly( Opcode::GetVariableArgument ,
                    argumentCountType , argumentCountIdx,
                    startIndexType , startIndexIdx ,
                    endIndexType , endIndexIdx
    );
}


u32 Helper::AddPlaceholder()
{
    u32 placeholderLocation = _bytecode.Size();
    _bytecode.PushBack(0 );
    return placeholderLocation;
}


bool Helper::IsPlaceHolder( u32 index )
{
    return _bytecode[ index ] == 0;
}


void Helper::ProcessBreakAndContinueStatementInLoopStatement( u32 loopBeginLocation , u32 loopEndLocation )
{
    Encoder::Helper encodeHelper( _context );
    auto UpdatePlaceHolderToJumpInstruction = [&]( u32 destinationLoopLocation ,
                                                   CompileContext::RetrievePlaceholderLocationFunction retrievePlaceholderLocationFunction )
    {
        Array< u32 > placeHolderLocations;
        ( _context->*retrievePlaceholderLocationFunction)( placeHolderLocations , loopBeginLocation , loopEndLocation );
        for( auto placeHolderLocation : placeHolderLocations )
        {
            if( encodeHelper.IsPlaceHolder( placeHolderLocation ) )
            {
                encodeHelper.JumpAlways( placeHolderLocation , destinationLoopLocation );
            }
        }
    };

    // update continue placeholder to jump to loop begin location
    UpdatePlaceHolderToJumpInstruction( loopBeginLocation , &CompileContext::RetrieveContinueLocation );
    // update break placeholder to jump to loop end location
    UpdatePlaceHolderToJumpInstruction( loopEndLocation , &CompileContext::RetrieveBreakLocation );
}


u64 MakeOperandIndex( s32 value )
{
    return value > 0 ? value : - value | 1 <<  OperandIndexBitCount - 1  ;
}


template<>
u64 MakeOperand( OperandType type, u64 value )
{
   return  ( OperandTypeBitMask & u64( type ) ) << OperandIndexBitCount |  ( value & OperandIndexBitMask );
}

u64 MakeInstruction(Opcode opcode, u64 destOperand , u64 srcOperand1 , u64 srcOperand2 )
{
    return ( ( OpcodeBitMask & u64( opcode ) ) << ( OperandBitCount * 3 ) )
           | ( (OperandBitMask & destOperand ) << (OperandBitCount * 2 ) )
           | ( (OperandBitMask & srcOperand1 ) << ( OperandBitCount ) )
           | ( (OperandBitMask & srcOperand2 ) );
}


void Encode( Array< u64 > &code , Opcode opcode , u64 destOperand , u64 srcOperand1 , u64 srcOperand2 )
{
    code.PushBack( Encode( opcode , destOperand , srcOperand1 , srcOperand2 ) );
}

u64 Encode( Opcode opcode , u64 destOperand , u64 srcOperand1 , u64 srcOperand2 )
{
    return MakeInstruction( opcode , destOperand , srcOperand1 , srcOperand2 );
}

} // Encoder
} // InstructionSet



} // LXX
