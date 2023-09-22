//
// Created by Harry on 9/13/2023.
//
#include "DisassemblyUtilities.h"
#include "InstructionDecoder.h"
#include <core/containers/UnorderedMap.h>
#include <core/vm/ByteCodeChunk.h>
#include <core/archive/iostream/IOStream.h>
#include <core/archive/MemoryArchiveWriter.h>
#include <core/log/log.h>
#include <core/objects/string/string.h>
#include <core/objects/string/StringUtil.h>
#include <core/debugger/DebuggerUtil.h>

namespace LXX
{
namespace Disassembly
{

static UnorderedMap< Opcode , const char* > _opcodeMaps =
{
        { Opcode::Call , "Call" } ,
        { Opcode::Return , "Return" } ,
        { Opcode::Jump ,                           "Jump" } ,
        { Opcode::SystemCall ,                     "SystemCall" } ,
        { Opcode::Assert  ,                        "Assert" } ,
        { Opcode::Push ,                           "Push" } ,
        { Opcode::Pop ,                            "Pop" } ,
        { Opcode::NewTable ,                       "NewTable" } ,
        { Opcode::GetField ,                       "GetField" } ,
        { Opcode::SetField ,                       "SetField" } ,
        { Opcode::GetVariableArgument ,            "GetVariableArgument" } ,
        { Opcode::Assignment ,                     "Assignment" } ,
        { Opcode::BinaryOpPower ,                  "BinaryOpPower" } ,
        { Opcode::BinaryOpCmpValueType ,           "BinaryOpCmpValueType" } ,
        { Opcode::BinaryOpCmpLessThan ,            "BinaryOpCmpLessThan" } ,
        { Opcode::BinaryOpCmpLessThanOrEqual,      "BinaryOpCmpLessThanOrEqual" } ,
        { Opcode::BinaryOpCmpEqual,                "BinaryOpCmpEqual" } ,
        { Opcode::BinaryOpCmpNotEqual,             "BinaryOpCmpNotEqual" } ,
        { Opcode::BinaryOpCmpGreaterThan ,         "BinaryOpCmpGreaterThan" } ,
        { Opcode::BinaryOpCmpGreaterThanOrEqual,   "BinaryOpCmpGreaterThanOrEqual" } ,
        { Opcode::BinaryOpAdd ,                    "BinaryOpAdd" } ,
        { Opcode::BinaryOpSubtract,                "BinaryOpSubtract" } ,
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


void PrintInstruction( State * state
        , ByteCodeChunk* chunk
        , Opcode opcode
        , OperandType operandType1 , u32 operandIndex1
        , OperandType operandType2 , u32 operandIndex2
        , OperandType operandType3 , u32 operandIndex3
)
{
    struct OperandInfo
    {
        char Name[16] = {0};
        char Value[16] = {0};
    };
    auto GetOperandInfo = [&]( OperandInfo &operandInfo , const OperandType type , const u32 index )
    {
        StringUtil::Strncpy( operandInfo.Name , _opcodeTypeMaps[type] , 15 );
        if( type == OperandType::Constant )
        {
            Value* constValue = chunk->GetConstValue( index );
            if( constValue->IsString() )
            {
                u32 cpLen = StringUtil::Strncpy(  operandInfo.Value + 1
                               , constValue->As<String*>()->GetData()
                               , 13 );
                operandInfo.Value[0]='\"';
                operandInfo.Value[ cpLen + 1 ]='\"';
            }
            else if( constValue->IsInteger() )
            {
                std::sprintf(
                        operandInfo.Value
                        ,"%d"
                        , constValue->As<s32>()
                        );
            }
            else if( constValue->IsReal() )
            {
                std::sprintf(
                        operandInfo.Value
                        ,"%f"
                        , constValue->As<f64>()
                );
            }
        }
        else if( type == OperandType::Immediate )
        {
            std::sprintf(
                    operandInfo.Value
                    ,"%d"
                    , index
            );
        }
        else if( type == OperandType::Stack
            || type == OperandType::TempVariable
            || type == OperandType::LocalVariable )
        {
            std::sprintf(
                    operandInfo.Value
                    ,"Index:%d"
                    , Decoder::GetOperandIndex( index )
            );
        }
        else if( type == OperandType::UpValue )
        {
            Value* identifierName = chunk->GetConstValue( index );
            assert( identifierName->IsString() );
            std::sprintf(
                    operandInfo.Value
                    ,"%s"
                    , identifierName->As<String*>()->GetData()
            );

        }
    };
    Array< OperandInfo > operandInfos;
    GetOperandInfo( *operandInfos.Add() , operandType1 , operandIndex1 );
    GetOperandInfo( *operandInfos.Add() , operandType2 , operandIndex2 );
    GetOperandInfo( *operandInfos.Add() , operandType3 , operandIndex3 );

#if GENERATE_DEBUGGER_SYMBOL
    String* sourceCodeLine = Debugger::DebuggerUtil::GetSourceCodeLine( state );
    LOG::LogDebug( LOG::LogCategory::LXX ,
              "%s(%s=%s,%s=%s,%s=%s) ; %s"
              , _opcodeMaps[opcode]
              , operandInfos[0].Name , operandInfos[0].Value
            , operandInfos[1].Name , operandInfos[1].Value
            , operandInfos[2].Name , operandInfos[2].Value
            , sourceCodeLine ? sourceCodeLine->GetData() : "none"
    );
#else
    LOG::LogDebug( LOG::LogCategory::LXX ,
              "%s(%s=%s,%s=%s,%s=%s)"
              , _opcodeMaps[opcode]
              , operandInfos[0].Name , operandInfos[0].Value
            , operandInfos[1].Name , operandInfos[1].Value
            , operandInfos[2].Name , operandInfos[2].Value
    );
#endif
}

} // namespace Disassembly
} // namespace LXX
