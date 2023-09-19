//
// Created by Harry on 9/3/2023.
//

#include <core/compiler/Compiler.h>
#include <core/Value.h>
#include <core/vm/Instructions/InstructionEncoder.h>
#include <core/objects/string/String.h>
#include <core/containers/UnorderedMap.h>
#include <core/objects/LuaClosure.h>
#include <core/compiler/Compiler.h>
#include <core/vm/ByteCodeChunk.h>
#include "core/compiler/AST/BlockStatement.h"
#include "core/compiler/AST/FunctionStatement.h"
#include <core/compiler/AST/StatementBase.h>
#include <core/compiler/AST/LocalStatement.h>
#include <core/compiler/AST/AssignmentStatement.h>
#include <core/compiler/AST/BreakStatement.h>
#include <core/compiler/AST/ContinueStatement.h>
#include <core/compiler/AST/WhileStatement.h>
#include <core/compiler/AST/RepeatStatement.h>
#include <core/compiler/AST/IfStatement.h>
#include <core/compiler/AST/ForLoopStatement.h>
#include <core/compiler/AST/TableConstructorStatement.h>
#include <core/compiler/AST/TableFieldStatement.h>
#include <core/compiler/AST/ReturnStatement.h>
#include <core/compiler/AST/SemicolonStatement.h>
#include <core/compiler/AST/FunctionNameStatement.h>
#include <core/compiler/AST/Expressions/IdentifierExpression.h>
#include <core/compiler/AST/Expressions/ExpressionListStatement.h>
#include <core/compiler/AST/Expressions/NilExpression.h>
#include <core/compiler/AST/Expressions/BooleanExpression.h>
#include <core/compiler/AST/Expressions/IntegerNumberExpression.h>
#include <core/compiler/AST/Expressions/RealNumberExpression.h>
#include <core/compiler/AST/Expressions/StringExpression.h>
#include <core/compiler/AST/Expressions/UnaryOperatorExpression.h>
#include <core/compiler/AST/Expressions/BinaryOperationExpression.h>
#include <core/compiler/AST/Expressions/DotExpression.h>
#include <core/compiler/AST/Expressions/FunctionCallExpression.h>
#include <core/compiler/AST/Expressions/VarExpression.h>
#include <core/compiler/AST/Expressions/VarListExpression.h>

namespace LXX
{


static void ProcessBreakAndContinueStatementInLoopStatement( CompileContext *context , u32 loopBeginLocation , u32 loopEndLocation )
{
    Encoder::Helper encodeHelper( context );
    auto UpdatePlaceHolderToJumpInstruction = [&]( u32 destinationLoopLocation ,
            CompileContext::RetrievePlaceholderLocationFunction retrievePlaceholderLocationFunction )
    {
        Array< u32 > placeHolderLocations;
        (context->*retrievePlaceholderLocationFunction)( placeHolderLocations , loopBeginLocation , loopEndLocation );
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


CompileContext::CompileContext( FunctionStatement *functionStatement ,  CompileContext *parent )
: _functionStatement( functionStatement )
, _parent( parent )
, _luaClosure( new LuaClosure( new ByteCodeChunk( functionStatement->FetchParameterNames() ) ) )
{

}

u32 CompileContext::AddTempVariable()
{
    ByteCodeChunk *chunk = _luaClosure->GetChunk();
    return chunk->AddTempVariable();
}


void CompileContext::GetVariableOperandInfo( String *variableName ,  OperandType &operandType , u32 &operandIndex )
{
    if( IsParameterVariable( variableName , &operandIndex ) )
    {
        operandType = OperandType::Stack;
    }
    else if( IsLocalVariable( variableName , &operandIndex ) )
    {
        operandType = OperandType::LocalVariable;
    }
    else
    {
        ByteCodeChunk *chunk = _luaClosure->GetChunk();
        operandType = OperandType::UpValue;
        operandIndex = chunk->AddConstValue( variableName );
    }
}


bool CompileContext::AddLocalVariable( String *name  , u32 *outIdx )
{
    ByteCodeChunk *chunk = _luaClosure->GetChunk();
    return chunk->AddLocalVariable( name , outIdx );
}


bool CompileContext::IsLocalVariable( String *name , u32 *outIdx )
{
    ByteCodeChunk *chunk = _luaClosure->GetChunk();
    return chunk->IsLocalVariable( name , outIdx );
}


bool CompileContext::IsParameterVariable( String *name , u32 *outIdx )
{
    ByteCodeChunk *chunk = _luaClosure->GetChunk();
    return chunk->IsParameterVariable( name );
}

void CompileContext::AddBreakLocation( u32 location )
{
    _breakStatementLocations.Add( location );
}

void CompileContext::AddContinueLocation( u32 location )
{
    _continueStatementLocations.Add( location );
}


void CompileContext::RetrieveBreakLocation( Array< u32 > &locations  , u32 beginLocation , u32 endLocation )
{
    RetrieveLocation( _breakStatementLocations , beginLocation , endLocation , locations );
}


void CompileContext::RetrieveContinueLocation( Array< u32 > &locations  , u32 beginLocation , u32 endLocation )
{
    RetrieveLocation( _continueStatementLocations , beginLocation , endLocation , locations );
}


void CompileContext::RetrieveLocation( const Array< u32 > &locations , u32 beginLocation , u32 endLocation , Array< u32 > &outLocations )
{
    for( auto location : locations )
    {
        if( location >= beginLocation && location <= endLocation )
            outLocations.Add( location );
    }
}

void Compiler::CompileFunctionCallExpression( CompileContext *context , FunctionCallExpression * functionCallExpression , u32 &argumentNumIdx )
{
    Encoder::Helper encodeHelper( context );

    ByteCodeChunk *chunk = context->GetLuaClosure()->GetChunk();

    auto functionName =   LXX::Cast< IdentifierExpression >( functionCallExpression->GetFunctionName() );

    auto varExpression = LXX::Cast< VarExpression >( functionCallExpression->GetVarExpression() );
    OperandType operandType = OperandType::None;
    auto CompileFunctionCallPrefix = [&]( u32 &outFunctionIdx , u32 &outSelfIdx )
    {
        outSelfIdx = -1;
        outFunctionIdx = context->AddTempVariable();
        bool isTable = false;
        u32 operandIndex = 0;
        CompileVarExpression( context , varExpression , operandType , operandIndex , isTable );
        if( isTable )
        {
            u32 tableIdx = context->GetLastTempVariableIndex();
            if( functionName != nullptr )
            {
                // for situation table.var2:functionName(...)
                encodeHelper.GetField( OperandType::TempVariable , tableIdx ,
                                       OperandType::TempVariable , tableIdx ,
                                       operandType , operandIndex );
                encodeHelper.GetField( OperandType::TempVariable , outFunctionIdx ,
                                       OperandType::TempVariable , tableIdx ,
                                       OperandType::Constant , chunk->AddConstValue( functionName->GetIdentifier() )
                                       );
                outSelfIdx = tableIdx;
            }
            else
            {
                // for situation table.var2(...)
                encodeHelper.GetField( OperandType::TempVariable , outFunctionIdx ,
                                       OperandType::TempVariable , tableIdx ,
                                       operandType , operandIndex );
            }
        }
        else
        {
            if( functionName != nullptr )
            {
                // for situation var1:FunctionName(...)
                encodeHelper.GetField( operandType , outFunctionIdx ,
                                       operandType , operandIndex ,
                                       OperandType::Constant , chunk->AddConstValue( functionName->GetIdentifier() )
                                       );
                outSelfIdx = operandIndex;
            }
            else
            {
                // for situation var1(...)
                encodeHelper.Assign( OperandType::TempVariable , outFunctionIdx ,
                                       operandType , operandIndex
                                       );
            }
        }
    };

    argumentNumIdx = context->AddTempVariable();
    u32 functionIdx = -1;
    u32 selfIdx = -1;
    CompileFunctionCallPrefix( functionIdx , selfIdx );
    encodeHelper.Push( OperandType::TempVariable , functionIdx );
    if( selfIdx != -1 )
        encodeHelper.Push( OperandType::TempVariable , selfIdx );

    // push arguments
    auto expressionList = LXX::Cast< ExpressionListStatement >( functionCallExpression->GetArguments() );
    if( expressionList != nullptr )
    {
        CompileComplicatedExpressionListStatement(context, expressionList);
        encodeHelper.Assign( OperandType::TempVariable , argumentNumIdx ,
        OperandType::Stack , Encoder::MakeOperandIndex( -1 ) );
        // pop arguments count
        encodeHelper.Pop( OperandType::Immediate , 1 );
        if( selfIdx != -1 )
        {
            encodeHelper.Increase(
                    OperandType::TempVariable , argumentNumIdx ,
                    1
            );
        }
    }
    else
    {
        encodeHelper.Assign( OperandType::TempVariable , argumentNumIdx ,
                             OperandType::Immediate , 0 );
    }

    // call function
    encodeHelper.Call( OperandType::TempVariable , argumentNumIdx ,
                       OperandType::TempVariable ,context->AddTempVariable() );
}


void Compiler::CompileVarExpression( CompileContext *context , VarExpression * varExpression , OperandType &operandType  , u32 &operandIndex , bool &isTable )
{
    Encoder::Helper encodeHelper( context );
    if( varExpression == nullptr )
        ThrowError( "invalid var expression" );

    ByteCodeChunk *chunk = context->GetLuaClosure()->GetChunk();

    Array< StatementBase * > & varStatements = varExpression->GetStatements();
    if( varStatements.Size() == 0 ||  varStatements[0] == nullptr
        || !varStatements[0]->IsA< IdentifierExpression >() )
        ThrowError( "invalid var expression" );

    auto identifier = LXX::Cast< IdentifierExpression >( varStatements[0] );

    OperandType destType = OperandType::UpValue;
    u32 destIdx = -1;
    context->GetVariableOperandInfo(  identifier->GetIdentifier() , destType , destIdx );

    isTable = varStatements.Size() > 1;

    if( !isTable )
    {
        operandType = destType;
        operandIndex = destIdx;
    }
    else
    {
        u32 tableIdx = context->AddTempVariable();
        encodeHelper.Assign( OperandType::TempVariable , tableIdx ,
                             destType , destIdx );
        auto CompileVarStatement = [&]( StatementBase *statement , OperandType &outType , u32 &outIdx )
        {
            if( auto identifierExpression = LXX::Cast< IdentifierExpression >( statement ) )
            {
                outType = OperandType::Constant;
                outIdx = chunk->AddConstValue( identifierExpression->GetIdentifier() );
            }
            else
            {
                outType = OperandType::TempVariable;
                CompileExpressionAndProcessReturnValue( context , statement );
                outIdx = context->GetLastTempVariableIndex();
            }
        };

        u32 keyIdx  = -1 ;
        OperandType keyType = OperandType::None;
        for(u32 iVarStatement = 1 ; iVarStatement < varStatements.Size() - 1 ; iVarStatement++ )
        {
            CompileVarStatement( varStatements[ iVarStatement ] , keyType , keyIdx );
            encodeHelper.GetField( OperandType::TempVariable , tableIdx ,
                                   OperandType::TempVariable , tableIdx ,
                                   keyType , keyIdx );
        }

        CompileVarStatement( varStatements.GetLast() , operandType , operandIndex );

        encodeHelper.Assign( OperandType::TempVariable , context->AddTempVariable() ,
                            OperandType::TempVariable , tableIdx );
    }
}


void Compiler::CompileWhileStatement( CompileContext *context , WhileStatement * whileStatement )
{
    Encoder::Helper encodeHelper( context );
    StatementBase *condition = whileStatement->GetCondition();
    StatementBase *body = whileStatement->GetBody();

    if( condition == nullptr || body == nullptr )
        ThrowError( "invalid while statement" );

    u32 conditionResIdx = -1;
    encodeHelper.EncodeWhileStatement(
            [&](Encoder::Helper &helper) {
                CompileExpressionAndProcessReturnValue(context, condition);
                conditionResIdx = context->GetLastTempVariableIndex();
            },
            OperandType::TempVariable, conditionResIdx,
            [&](Encoder::Helper &helper) {
                CompileStatement(context, body);
            }
    );
}


void Compiler::CompileContinueStatement( CompileContext *context , ContinueStatement * continueStatement )
{
    Encoder::Helper encodeHelper( context );
    context->AddContinueLocation( encodeHelper.AddPlaceholder() );
}


void Compiler::CompileBreakStatement( CompileContext *context , BreakStatement * breakStatement )
{
    Encoder::Helper encodeHelper( context );
    context->AddBreakLocation( encodeHelper.AddPlaceholder() );
}


void Compiler::CompileRepeatStatement( CompileContext *context , RepeatStatement * repeatStatement )
{
    Encoder::Helper encodeHelper( context );
    StatementBase *condition = repeatStatement->GetCondition();
    StatementBase *body = repeatStatement->GetBody();

    if( condition == nullptr || body == nullptr )
        ThrowError( "invalid while statement" );

    u32 conditionResIdx = -1;
    encodeHelper.EncodeRepeatStatement(
            [&](Encoder::Helper &helper) {
                CompileExpressionAndProcessReturnValue(context, condition);
                conditionResIdx = context->GetLastTempVariableIndex();
            },
            OperandType::TempVariable, conditionResIdx,
            [&](Encoder::Helper &helper) {
                CompileStatement(context, body);
            }
    );
}

void Compiler::CompileIfStatement( CompileContext *context , IfStatement * ifStatement )
{
    Encoder::Helper encodeHelper( context );
    ByteCodeChunk *chunk = context->GetLuaClosure()->GetChunk();
    Array< u64 > &code = chunk->GetCode();

    auto condition = ifStatement->GetCondition();
    auto thenBody = ifStatement->GetThen();
    auto elseBody = ifStatement->GetElse();

    if( condition == nullptr || thenBody == nullptr )
        ThrowError( "invalid if statement" );

    Array< StatementBase * > conditions;
    Array< StatementBase * > thenBodies;
    conditions.PushBack( condition );
    thenBodies.PushBack( thenBody );

    Array< StatementBase * > elseIfStatements ;
    ifStatement->GetElseif( elseIfStatements );
    for( auto elseIfStatement : elseIfStatements )
    {
        if( auto statement = LXX::Cast< IfStatement >( elseIfStatement ) )
        {
            conditions.PushBack( statement->GetCondition() );
            thenBodies.PushBack( statement->GetThen() );
        }
        else
        {
            ThrowError( "invalid elseif statement" );
        }
    }

    Array< u32 > breakLocations;
    for( u32 i = 0 ; i < conditions.Size() ; ++ i )
    {
        CompileExpressionAndProcessReturnValue( context , conditions[ i ] );
        u32 conditionResIdx = context->GetLastTempVariableIndex();
        encodeHelper.UnaryNot( OperandType::TempVariable , conditionResIdx ,
                               OperandType::TempVariable , conditionResIdx );
        u32 jumpLocation = code.Size();
        code.PushBack( 0 );
        CompileStatement( context , thenBodies[ i ] );
        breakLocations.PushBack( code.Size() );
        code.PushBack( 0 );
        encodeHelper.Jump(Encoder::MakeOperand( OperandType::TempVariable , conditionResIdx ) ,
                        jumpLocation ,
                code.Size() );
    }

    // else block
    if( elseBody != nullptr )
    {
        CompileStatement( context , elseBody );
    }

    // update break location to make sure it can jump to the end of the if statement
    for( auto breakLocation : breakLocations )
    {
        encodeHelper.JumpAlways(breakLocation ,
                            code.Size() );
    }

}


void Compiler::CompileForLoopStatement( CompileContext *context , ForLoopStatement * forLoopStatement )
{
    Encoder::Helper encodeHelper( context );
    ByteCodeChunk * chunk = context->GetLuaClosure()->GetChunk();

    auto condition = LXX::Cast< AssignmentStatement >( forLoopStatement->GetCondition() );
    if( condition == nullptr )
        ThrowError( "invalid for loop statement , for loop condition is null" );

    auto body = forLoopStatement->GetBody();
    if( body == nullptr )
        ThrowError( "invalid for loop statement , for loop body is null" );


    VarListExpression * varListExpression = LXX::Cast< VarListExpression >( condition->GetLeft() );
    if( varListExpression == nullptr )
        ThrowError( "invalid for loop statement , for loop condition left is not a var list expression" );

    Array< IdentifierExpression * > identifierExpressions;

    Array< VarExpression * > &varExpressions = varListExpression->GetVarExpressions();
    for( u32 iVar = 0 ; iVar < varExpressions.Size() ; iVar++ )
    {
        VarExpression * varExpression = varExpressions[ iVar ];
        if( varExpression == nullptr )
            ThrowError( "invalid for loop statement , for loop condition left is null" );
        Array< StatementBase * > &statements = varExpression->GetStatements();
        if( statements.Size() != 1 )
            ThrowError( "invalid for loop statement , for loop condition left is not a single statement" );
        IdentifierExpression * identifierExpression = LXX::Cast< IdentifierExpression >( statements[ 0 ] );

        if( identifierExpression == nullptr )
            ThrowError( "invalid for loop statement , for loop condition left is not a single identifier expression" );

        identifierExpressions.PushBack( identifierExpression );
    }

    Array< u32 > varExpressionIndices;
    Array< OperandType > varExpressionTypes;

    for( u32 iVar = 0 ; iVar < identifierExpressions.Size() ; iVar++ )
    {
        IdentifierExpression * identifierExpression = identifierExpressions[ iVar ];
        OperandType varExpressionType = OperandType::None ;
        u32 varExpressionIndex = -1 ;
        context->GetVariableOperandInfo( identifierExpression->GetIdentifier() , varExpressionType , varExpressionIndex );
        varExpressionTypes.PushBack( varExpressionType );
        varExpressionIndices.PushBack( varExpressionIndex );
    }

    u32 conditionResIdx = -1;
    u32 firstExpressionValueIdx = context->AddTempVariable();
    u32 secondExpressionValueIdx = context->AddTempVariable();
    u32 thirdExpressionValueIdx = context->AddTempVariable();
    u32 fourthExpressionValueIdx = context->AddTempVariable();
    u32 varExpressionCount = identifierExpressions.Size();
    u32 increaseStepIdx = context->AddTempVariable();
    encodeHelper.EncodeForLoopStatement(
            [&](Encoder::Helper &helper) {
                /*
                 *  here is the initialization part before the loop
                 *  we try to compile the right part of the condition as expression list statement,
                 *  and we check the expression value count from the top of the stack
                 *  1. if the count is less than to equal to zero , we raise a system exception ( index : -1 )
                 *  2. if the count is one , we fetch the value from the stack ( index : -2 )
                 *  3. if the count is two , we fetch the value from the stack ( index : -3 )
                 *  4. if the count is three , we fetch the value from the stack ( index : -4 )
                 *      ...
                 */
                helper.Assign(
                         OperandType::TempVariable , firstExpressionValueIdx ,
                         OperandType::Constant , chunk->GetConstNilValueIndex()
                        );
                helper.Assign(
                        OperandType::TempVariable , secondExpressionValueIdx ,
                        OperandType::Constant , chunk->GetConstNilValueIndex()
                        );
                helper.Assign(
                        OperandType::TempVariable , thirdExpressionValueIdx ,
                        OperandType::Constant , chunk->GetConstNilValueIndex()
                        );
                helper.Assign(
                        OperandType::TempVariable , fourthExpressionValueIdx ,
                        OperandType::Constant , chunk->GetConstNilValueIndex()
                        );
                CompileComplicatedExpressionListStatement(context,
                                                          LXX::Cast<ExpressionListStatement>(condition->GetRight()));
                helper.EncodeSimpleIfStatement(
                    [&]( Encoder::Helper &helper )
                    {
                        helper.EncodeDirectly(
                                Opcode::BinaryOpCmpLessThanOrEqual,
                                OperandType::TempVariable , conditionResIdx ,
                                OperandType::Stack , Encoder::MakeOperandIndex( -1 ) ,
                                OperandType::Immediate , 0 );
                    }
                    ,OperandType::TempVariable , conditionResIdx ,
                    [&]( Encoder::Helper &helper )
                    {
                        helper.RaiseException( SystemException::ExceptionForZeroReturnValueCount );
                    }
                );
                helper.Assign( OperandType::TempVariable , firstExpressionValueIdx ,
                               OperandType::Stack , Encoder::MakeOperandIndex( -2 ) );
                Array< u32 > jumpToEndLocations;
                helper.EncodeSimpleIfStatement(
                        [&]( Encoder::Helper &helper )
                        {
                            helper.EncodeDirectly(
                                    Opcode::BinaryOpCmpEqual,
                                    OperandType::TempVariable , conditionResIdx ,
                                    OperandType::Stack , Encoder::MakeOperandIndex( -1 ) ,
                                    OperandType::Immediate , 1 );
                        }
                        ,OperandType::TempVariable , conditionResIdx ,
                        [&]( Encoder::Helper &helper )
                        {
                            jumpToEndLocations.PushBack( helper.AddPlaceholder() );
                        }
                );
                helper.Assign( OperandType::TempVariable , secondExpressionValueIdx ,
                               OperandType::Stack , Encoder::MakeOperandIndex( -3 ) );
                helper.EncodeSimpleIfStatement(
                        [&]( Encoder::Helper &helper )
                        {
                            helper.EncodeDirectly(
                                    Opcode::BinaryOpCmpEqual,
                                    OperandType::TempVariable , conditionResIdx ,
                                    OperandType::Stack , Encoder::MakeOperandIndex( -1 ) ,
                                    OperandType::Immediate , 2 );
                        }
                        ,OperandType::TempVariable , conditionResIdx ,
                        [&]( Encoder::Helper &helper )
                        {
                            jumpToEndLocations.PushBack( helper.AddPlaceholder() );
                        }
                );
                helper.Assign( OperandType::TempVariable , thirdExpressionValueIdx ,
                               OperandType::Stack , Encoder::MakeOperandIndex( -4 ) );
                helper.EncodeSimpleIfStatement(
                        [&]( Encoder::Helper &helper )
                        {
                            helper.EncodeDirectly(
                                    Opcode::BinaryOpCmpEqual,
                                    OperandType::TempVariable , conditionResIdx ,
                                    OperandType::Stack , Encoder::MakeOperandIndex( -1 ) ,
                                    OperandType::Immediate , 3 );
                        }
                        ,OperandType::TempVariable , conditionResIdx ,
                        [&]( Encoder::Helper &helper )
                        {
                            jumpToEndLocations.PushBack( helper.AddPlaceholder() );
                        }
                );
                helper.Assign( OperandType::TempVariable , fourthExpressionValueIdx ,
                               OperandType::Stack , Encoder::MakeOperandIndex( -5 ) );
                for( auto jumpToEndLocation : jumpToEndLocations )
                {
                    helper.JumpAlways( jumpToEndLocation ,helper.GetNextInstructionLocation() );
                }
                // pop out all expression values and the expression count from the stack
                helper.Pop( OperandType::Stack , Encoder::MakeOperandIndex( -1 ) );
                helper.Pop( OperandType::Immediate , 1 );

                // process numerical for loop
                helper.EncodeSimpleIfStatement(
                        [&]( Encoder::Helper &helper )
                        {
                            helper.CompareValueType(
                                    OperandType::TempVariable , conditionResIdx ,
                                    OperandType::TempVariable , firstExpressionValueIdx ,
                                    ValueType::Number );
                        }
                        ,OperandType::TempVariable , conditionResIdx ,
                        [&]( Encoder::Helper &helper )
                        {

                            // if the second expression is not number , raise exception
                            helper.EncodeSimpleIfStatement(
                                    [&]( Encoder::Helper &helper )
                                    {
                                        helper.CompareValueType(
                                                OperandType::TempVariable , conditionResIdx ,
                                                OperandType::TempVariable , secondExpressionValueIdx ,
                                                ValueType::Number );
                                        helper.UnaryNot(
                                                OperandType::TempVariable , conditionResIdx,
                                                OperandType::TempVariable , conditionResIdx
                                        );
                                    }
                                    ,OperandType::TempVariable , conditionResIdx ,
                                    [&]( Encoder::Helper &helper )
                                    {
                                        helper.RaiseException( SystemException::ExceptionForStatementLimitExpressionIsNotNumber );
                                    }
                            );
                            helper.Assign(
                                    OperandType::TempVariable , increaseStepIdx ,
                                    OperandType::Immediate , 1
                            );
                            helper.EncodeSimpleIfStatement(
                                    [&]( Encoder::Helper &helper )
                                    {
                                        helper.CompareValueType(
                                                OperandType::TempVariable , conditionResIdx ,
                                                OperandType::TempVariable , thirdExpressionValueIdx ,
                                                ValueType::Number );
                                    }
                                    ,OperandType::TempVariable , conditionResIdx ,
                                    [&]( Encoder::Helper &helper )
                                    {
                                        helper.Assign(
                                                OperandType::TempVariable , increaseStepIdx ,
                                                OperandType::TempVariable , thirdExpressionValueIdx
                                        );
                                    },
                                    [&]( Encoder::Helper &helper )
                                    {
                                        helper.EncodeSimpleIfStatement(
                                                [&]( Encoder::Helper &helper )
                                                {
                                                    helper.EncodeDirectly(
                                                            Opcode::BinaryOpCmpEqual,
                                                            OperandType::TempVariable , conditionResIdx ,
                                                            OperandType::TempVariable , thirdExpressionValueIdx ,
                                                            OperandType::Constant , chunk->GetConstNilValueIndex()
                                                    );
                                                    helper.UnaryNot(
                                                            OperandType::TempVariable , conditionResIdx,
                                                            OperandType::TempVariable , conditionResIdx
                                                    );
                                                }
                                                , OperandType::TempVariable , conditionResIdx ,
                                                [&]( Encoder::Helper &helper )
                                                {
                                                    helper.RaiseException( SystemException::ExceptionForStatementIncreaseStepIsNotNumberAndIsNotNil );
                                                }
                                        );
                                    }
                            );

                            // raise exception if the increase step is equal zero
                            helper.EncodeSimpleIfStatement(
                                    [&]( Encoder::Helper &helper )
                                    {
                                        helper.EncodeDirectly(
                                                Opcode::BinaryOpCmpEqual ,
                                                OperandType::TempVariable , conditionResIdx ,
                                                OperandType::TempVariable , increaseStepIdx ,
                                                OperandType::Immediate , 0
                                        );
                                    }
                                    ,OperandType::TempVariable , conditionResIdx ,
                                    [&]( Encoder::Helper &helper )
                                    {
                                        helper.RaiseException( SystemException::ExceptionForStatementIncreaseStepIsEqualZero );
                                    }
                            );
                        }
                );
            },
            [&](Encoder::Helper &helper) {

                /*
                 *  if the first expression is a function ,
                 *      we call it with
                 *          1. the second expression value idx as the state
                 *          2. the third expression value idx as the control variable
                 * */
                helper.EncodeSimpleIfStatement(
                        [&]( Encoder::Helper &helper )
                        {
                            helper.CompareValueType(
                                    OperandType::TempVariable , conditionResIdx ,
                                    OperandType::TempVariable , firstExpressionValueIdx ,
                                    ValueType::Function );
                        }
                        ,OperandType::TempVariable , conditionResIdx ,
                        [&]( Encoder::Helper &helper )
                        {
                            //
                            helper.Push( OperandType::TempVariable , firstExpressionValueIdx );
                            helper.Push( OperandType::TempVariable , secondExpressionValueIdx );
                            helper.Push( OperandType::TempVariable , thirdExpressionValueIdx );
                            u32 returnValueCountIdx = context->AddTempVariable();
                            helper.Call( 2 , OperandType::TempVariable , returnValueCountIdx );

                            u32 iteratorIdx = context->AddTempVariable();
                            helper.Assign(
                                    OperandType::TempVariable , iteratorIdx ,
                                    OperandType::Immediate , 0
                            );

                            helper.EncodeWhileStatement(
                                    [&]( Encoder::Helper &helper )
                                    {
                                        helper.EncodeDirectly(
                                                Opcode::BinaryOpCmpLessThanOrEqual,
                                                OperandType::TempVariable , conditionResIdx ,
                                                OperandType::TempVariable , iteratorIdx ,
                                                OperandType::Immediate , varExpressionCount
                                                );
                                    },
                                    OperandType::TempVariable , conditionResIdx ,
                                    [&]( Encoder::Helper &helper )
                                    {
                                        for( u32 iVar = 0 ; iVar < varExpressionIndices.Size() ; iVar++ )
                                        {
                                            u32 varExpressionIdx = varExpressionIndices[ iVar ];
                                            helper.EncodeSimpleIfStatement(
                                                    [&]( Encoder::Helper &helper )
                                                    {
                                                        helper.EncodeDirectly(
                                                                Opcode::BinaryOpCmpLessThan ,
                                                                OperandType::TempVariable , conditionResIdx ,
                                                                OperandType::Immediate , iVar ,
                                                                OperandType::TempVariable , returnValueCountIdx
                                                        );
                                                    },
                                                    OperandType::TempVariable , conditionResIdx ,
                                                    [&]( Encoder::Helper &helper )
                                                    {
                                                        helper.Assign(
                                                                OperandType::TempVariable , varExpressionIdx ,
                                                                OperandType::Stack , Encoder::MakeOperandIndex( -1 - iVar )
                                                        );
                                                    },
                                                    [&]( Encoder::Helper &helper )
                                                    {
                                                        helper.Assign(
                                                                OperandType::TempVariable , varExpressionIdx ,
                                                                OperandType::Constant , chunk->GetConstNilValueIndex()
                                                        );
                                                    }
                                            );
                                        }
                                    }
                                    );
                        }
                );

                /*
                 *  if the first expression is number ,
                 *      we expect the second expression is number as the limit
                 *      and the third expression is number as the increase step
                 */
                helper.EncodeSimpleIfStatement(
                        [&]( Encoder::Helper &helper )
                        {
                            helper.CompareValueType(
                                    OperandType::TempVariable , conditionResIdx ,
                                    OperandType::TempVariable , firstExpressionValueIdx ,
                                    ValueType::Number );
                        }
                        ,OperandType::TempVariable , conditionResIdx ,
                        [&]( Encoder::Helper &helper )
                        {

                            helper.EncodeSimpleIfStatement(
                                    [&]( Encoder::Helper &helper )
                                    {
                                        helper.EncodeDirectly(
                                                Opcode::BinaryOpCmpLessThan ,
                                                OperandType::TempVariable , conditionResIdx ,
                                                OperandType::TempVariable , increaseStepIdx ,
                                                OperandType::Immediate , 0
                                        );
                                    }
                                    ,OperandType::TempVariable , conditionResIdx ,
                                    [&]( Encoder::Helper &helper )
                                    {
                                        // increase step < 0
                                        helper.EncodeDirectly(
                                                Opcode::BinaryOpCmpGreaterThanOrEqual ,
                                                OperandType::TempVariable , conditionResIdx ,
                                                OperandType::TempVariable , varExpressionIndices[0] ,
                                                OperandType::TempVariable , secondExpressionValueIdx
                                        );

                                    }
                                    ,[&]( Encoder::Helper &helper )
                                    {
                                        // increase step < 0
                                        helper.EncodeDirectly(
                                                Opcode::BinaryOpCmpLessThanOrEqual ,
                                                OperandType::TempVariable , conditionResIdx ,
                                                OperandType::TempVariable , varExpressionIndices[0] ,
                                                OperandType::TempVariable , secondExpressionValueIdx
                                        );
                                    }
                            );

                        }
                );
            },
            OperandType::TempVariable, conditionResIdx,
            [&](Encoder::Helper &helper) {
                CompileStatement( context, body );
                helper.EncodeSimpleIfStatement(
                        [&](Encoder::Helper &helper) {
                            helper.CompareValueType(
                                    OperandType::TempVariable , conditionResIdx ,
                                    OperandType::TempVariable , firstExpressionValueIdx ,
                                    ValueType::Number );
                        }
                        ,OperandType::TempVariable , conditionResIdx ,
                        [&](Encoder::Helper &helper) {
                            helper.EncodeDirectly(
                                    Opcode::BinaryOpAdd,
                                    OperandType::TempVariable , varExpressionIndices[0] ,
                                    OperandType::TempVariable , varExpressionIndices[0] ,
                                    OperandType::TempVariable , increaseStepIdx
                                    );
                        }
                );
            }
    );

}

void Compiler::CompileReturnStatement( CompileContext *context , ReturnStatement * returnStatement )
{
    Encoder::Helper encodeHelper( context );
    ByteCodeChunk * chunk = context->GetLuaClosure()->GetChunk();

    auto expressionList = LXX::Cast< ExpressionListStatement >( returnStatement->GetReturnValue() );

    u32 returnValueCountIdx = context->AddTempVariable();

    if( expressionList != nullptr )
    {
        CompileComplicatedExpressionListStatement(context, expressionList);
        encodeHelper.Assign( OperandType::TempVariable , returnValueCountIdx ,
                             OperandType::Stack , Encoder::MakeOperandIndex( -1 ) );
        // pop out argument count
        encodeHelper.Pop( OperandType::Immediate , 1 );
    }

    encodeHelper.Return( OperandType::TempVariable , returnValueCountIdx );
}


void Compiler::CompileIdentifierExpression( CompileContext *context , IdentifierExpression * identifierExpression )
{
    Encoder::Helper encodeHelper( context );
    ByteCodeChunk *chunk = context->GetLuaClosure()->GetChunk();
    u32 identifierIdx = -1;
    OperandType identifierType = OperandType::None;
    if( chunk->IsLocalVariable( identifierExpression->GetIdentifier() , &identifierIdx ) )
    {
        identifierType = OperandType::LocalVariable;
    }
    else
    {
        identifierType = OperandType::UpValue;
        identifierIdx = chunk->AddConstValue( identifierExpression->GetIdentifier() );
    }
    encodeHelper.Assign(
        OperandType::TempVariable , context->AddTempVariable() ,
         identifierType , identifierIdx
            );
}


bool Compiler::IsComplicatedExpressionListStatement( ExpressionListStatement * expressionListStatement )
{
    if( expressionListStatement == nullptr )
        ThrowError( "invalid assignment , except expression list" );

    Array< StatementBase * > &expressions = expressionListStatement->GetExpressions();
    for( s32 i = expressions.Size() - 1 ; i >= 0 ; --i )
    {
        if (expressions[i]->IsA<FunctionCallExpression>() ||
            expressions[i]->IsA<DotExpression>()) {
            return true;
        }
    }
    return false;
}


void Compiler::CompileComplicatedExpressionListStatement(CompileContext *context , ExpressionListStatement * expressionListStatement )
{
    Encoder::Helper encodeHelper( context );
    ByteCodeChunk * chunk = context->GetLuaClosure()->GetChunk();

    if( expressionListStatement == nullptr )
        ThrowError( "invalid assignment , except expression list" );


    Array< StatementBase * > &expressions = expressionListStatement->GetExpressions();
    u32 expressionValueCountIndex = context->AddTempVariable();
    encodeHelper.Assign( OperandType::TempVariable , expressionValueCountIndex ,
                         OperandType::Immediate , 0 );

    Array< u64 > valueNeedToPop;
    Array< u32 > tempExpressions;
    for( s32 i = expressions.Size() - 1 ; i >= 0 ; --i )
    {
        CompileExpression( context , expressions[ i ] );
        if( expressions[ i ]->IsA< FunctionCallExpression >() ||
                expressions[ i ]->IsA< DotExpression >() )
        {
            u32 returnValueCountIdx = context->GetLastTempVariableIndex();
            u32 testConditionIdx = context->AddTempVariable();
            encodeHelper.EncodeSimpleIfStatement(
                    [&]( Encoder::Helper &helper )
                    {
                        /*
                         *  test if the return value count is zero ,
                         * */
                        encodeHelper.EncodeDirectly( Opcode::BinaryOpCmpEqual ,
                                                     OperandType::TempVariable , testConditionIdx ,
                                                     OperandType::TempVariable , returnValueCountIdx ,
                                                     OperandType::None , 0 );
                    }
                    ,OperandType::TempVariable , testConditionIdx ,
                    [&]( Encoder::Helper &helper ) {
                        /*
                         *  if true,
                         *  1. we push nil to the stack
                         *  2. increase the expression value count by one
                         * */
                        encodeHelper.Increase( OperandType::TempVariable , expressionValueCountIndex , 1 );
                        encodeHelper.Push( OperandType::Constant , chunk->GetConstNilValueIndex() );
                    }
                    , [&]( Encoder::Helper &helper ) {
                        /*
                         *  if false,
                         *  increase the expression value count by the return value count
                         * */
                        encodeHelper.EncodeDirectly( Opcode::BinaryOpAdd ,
                                                     OperandType::TempVariable , expressionValueCountIndex ,
                                                     OperandType::TempVariable , expressionValueCountIndex ,
                                                     OperandType::TempVariable , returnValueCountIdx );
                    }
            );
        }
        else
        {
            encodeHelper.Increase( OperandType::TempVariable , expressionValueCountIndex , 1 );
            encodeHelper.Push( OperandType::TempVariable , context->GetLastTempVariableIndex() );
        }
    }

    encodeHelper.Push( OperandType::TempVariable , expressionValueCountIndex );
}


void Compiler::CompileDotExpression( CompileContext *context , DotExpression * dotExpression )
{
    Encoder::Helper encodeHelper( context );
    ByteCodeChunk * chunk = context->GetLuaClosure()->GetChunk();

    u32 startIdx = context->AddTempVariable();
    u32 endIdx = context->AddTempVariable();
    u32 variableArgumentCountIdx = context->AddTempVariable();

    encodeHelper.GetVariableArgument(
            OperandType::TempVariable , startIdx ,
            OperandType::TempVariable , endIdx ,
            OperandType::TempVariable , variableArgumentCountIdx
    );

    u32 curIdx = startIdx;
    u32 testResultIdx = context->AddTempVariable();
    u32 copiedValueFromStack = context->AddTempVariable();

    encodeHelper.EncodeWhileStatement(
            [&](Encoder::Helper &helper) {
                encodeHelper.EncodeDirectly(
                        Opcode::BinaryOpCmpLessThanOrEqual ,
                        OperandType::TempVariable , testResultIdx ,
                        OperandType::TempVariable , curIdx ,
                        OperandType::TempVariable , endIdx
                );
            },
            OperandType::TempVariable , testResultIdx ,
            [&](Encoder::Helper &helper) {
                encodeHelper.Assign( OperandType::TempVariable , copiedValueFromStack ,
                                     OperandType::Stack , curIdx );
                encodeHelper.Push( OperandType::TempVariable , copiedValueFromStack );
                encodeHelper.Increase( OperandType::TempVariable , curIdx , 1 );
            }
    );

    encodeHelper.Assign( OperandType::TempVariable , context->AddTempVariable() ,
                         OperandType::TempVariable , variableArgumentCountIdx );
}


void Compiler::CompileTableConstructorStatement( CompileContext *context , TableConstructorStatement * tableConstructorStatement )
{
    Encoder::Helper encodeHelper( context );
    // create table
    ByteCodeChunk *chunk = context->GetLuaClosure()->GetChunk();
    u32 tableIdx = context->AddTempVariable();
    OperandType tableType = OperandType::TempVariable;
    encodeHelper.NewTable( tableType , tableIdx );

    Array< StatementBase * >&  fields = tableConstructorStatement->GetFields();

    s32 positionIdx = 1;
    for( u32 i = 0 ; i < fields.Size() ; i++ )
    {
        if( auto  tableFieldStatement = LXX::Cast< TableFieldStatement >( fields[i] ) )
        {
            StatementBase *keyStatement = tableFieldStatement->GetKey();
            StatementBase *valueStatement = tableFieldStatement->GetValue();
            OperandType keyType = OperandType::None;
            u32 keyIdx = -1;
            if( keyStatement == nullptr )
            {
                keyIdx = chunk->AddConstValue( positionIdx ++  );
                keyType = OperandType::Constant;
            }
            else
            {
                CompileExpressionAndProcessReturnValue( context , keyStatement );
                keyIdx = chunk->GetLastTempVariableIndex();
                keyType = OperandType::TempVariable;
            }

            if( valueStatement == nullptr )
                ThrowError( "Invalid table constructor , value expected" );

            CompileExpressionAndProcessReturnValue( context , valueStatement );
            u32 valueIdx = chunk->GetLastTempVariableIndex();
            OperandType valueType = OperandType::TempVariable;

            encodeHelper.SetField(  tableType , tableIdx , keyType , keyIdx , valueType , valueIdx );
        }
        else
        {
            ThrowError( "Invalid table constructor , TableFieldStatement expected" );
        }
    }

    encodeHelper.Assign( OperandType::TempVariable , context->AddTempVariable() ,
                                             tableType , tableIdx );
}


void Compiler::CompileExpression(CompileContext *context , StatementBase * statement )
{
    Encoder::Helper encodeHelper( context );
    ByteCodeChunk *chunk = context->GetLuaClosure()->GetChunk();

    if( auto unaryExpression = LXX::Cast< UnaryOperatorExpression >( statement ) )
    {
        CompileUnaryExpression( context , unaryExpression );
    }
    else if( auto compoundExpression = LXX::Cast< BinaryOperationExpression >( statement ) )
    {
        CompileBinaryExpression( context , compoundExpression );
    }
    else if( auto nilExpression = LXX::Cast< NilExpression >( statement ) )
    {
        encodeHelper.Assign(OperandType::TempVariable ,
                    context->AddTempVariable() ,
                    OperandType::Constant ,
                    chunk->GetConstNilValueIndex()
                    );
    }
    else if( auto booleanExpression = LXX::Cast< BooleanExpression >( statement ) )
    {
        encodeHelper.Assign(OperandType::TempVariable ,
                    context->AddTempVariable() ,
                    OperandType::Constant ,
                    chunk->AddConstValue( booleanExpression->GetValue() )
        );
    }
    else if( auto integerNumberExpression = LXX::Cast< IntegerNumberExpression >( statement ) )
    {
        encodeHelper.Assign( OperandType::TempVariable ,
                    context->AddTempVariable() ,
                    OperandType::Constant ,
                    chunk->AddConstValue( integerNumberExpression->GetValue() )
        );
    }
    else if( auto realNumberExpression = LXX::Cast< RealNumberExpression >( statement ) )
    {
        encodeHelper.Assign( OperandType::TempVariable ,
                    context->AddTempVariable() ,
                    OperandType::Constant ,
                    chunk->AddConstValue( realNumberExpression->GetValue() )
        );
    }
    else if( auto stringExpress = LXX::Cast< StringExpression >(statement ) )
    {
        encodeHelper.Assign( OperandType::TempVariable ,
                    context->AddTempVariable() ,
                    OperandType::Constant ,
                    chunk->AddConstValue( stringExpress->GetValue() )
        );
    }
    else if( auto tableConstructor = LXX::Cast< TableConstructorStatement >( statement ) )
    {
        CompileTableConstructorStatement( context , tableConstructor );
    }
    else if( auto varExpression = LXX::Cast< VarExpression >( statement ) )
    {
        OperandType operandType = OperandType::None;
        u32 operandIndex = -1;
        bool bisTable  = false;
        CompileVarExpression( context , varExpression , operandType , operandIndex , bisTable );
        if( bisTable )
        {
            u32 tableIdx = context->GetLastTempVariableIndex();
            encodeHelper.GetField(OperandType::TempVariable , context->AddTempVariable()
                                    ,OperandType::TempVariable , tableIdx
                                    ,operandType , operandIndex
                                    );
        }
        else
        {
            encodeHelper.Assign(OperandType::TempVariable ,context->AddTempVariable(),
                                operandType , operandIndex );
        }
    }
    else if( auto functionCallExpression = LXX::Cast< FunctionCallExpression >( statement ) )
    {
        u32 argumentNumIdx;
        CompileFunctionCallExpression( context , functionCallExpression ,  argumentNumIdx );
    }
    else if( auto functionStatement = LXX::Cast< FunctionStatement >( statement ) )
    {
        CompileFunctionStatementIgnoreName( context, functionStatement );
    }
    else if( auto identifierExpression = LXX::Cast< IdentifierExpression >( statement ) )
    {
        CompileIdentifierExpression( context , identifierExpression );
    }
    else if( auto dotExpression = LXX::Cast< DotExpression >( statement ) )
    {
        CompileDotExpression( context , dotExpression );
    }
    else
    {
        ThrowError( "invalid expression" );
    }
}


void Compiler::CompileExpressionAndProcessReturnValue( CompileContext *context , StatementBase * statement )
{
    CompileExpression( context , statement );
    if( statement->IsA< FunctionCallExpression >() || statement->IsA< DotExpression >() )
    {
        Encoder::Helper encodeHelper( context );
        ByteCodeChunk *chunk = context->GetLuaClosure()->GetChunk();
        u32 returnValueCountIdx = context->GetLastTempVariableIndex();
        u32 testConditionIdx = context->AddTempVariable();
        encodeHelper.EncodeSimpleIfStatement(
                [&]( Encoder::Helper &helper )
                {
                    // test if the return value count is equal to zero
                    encodeHelper.EncodeDirectly( Opcode::BinaryOpCmpEqual ,
                                                 OperandType::TempVariable , testConditionIdx ,
                                                 OperandType::TempVariable , returnValueCountIdx ,
                                                 OperandType::Immediate , 0 );
                }
                ,OperandType::TempVariable , testConditionIdx ,
                [&]( Encoder::Helper &helper )
                {
                    // if the return value count is equal to zero , assign nil to the return value
                    encodeHelper.Assign( OperandType::TempVariable , context->AddTempVariable() ,
                                         OperandType::Constant , chunk->GetConstNilValueIndex() );
                }
                ,[&]( Encoder::Helper &helper )
                {
                    /*
                     * if the return value count is greater than zero ,
                     *      1. fetch the first return value from stack
                     *      2. pop all  return values from stack
                     * **/
                    encodeHelper.Assign( OperandType::TempVariable , context->AddTempVariable() ,
                                         OperandType::Stack , Encoder::MakeOperandIndex( -2 ) );
                    encodeHelper.Pop( OperandType::TempVariable , returnValueCountIdx );
                }
        );
    }
}


void Compiler::CompileUnaryExpression( CompileContext *context , UnaryOperatorExpression * unaryExpression )
{
    Encoder::Helper encodeHelper( context );
    static const UnorderedMap< u32 , Opcode > unaryOperatorMap = {
            { '~' , Opcode::UnaryBitwiseNot } ,
            { '-' , Opcode::UnaryMinus } ,
            { Token::TK_NOT , Opcode::UnaryNot } ,
            { '#' , Opcode::UnaryLen } ,
    };

    s32 tokenType = unaryExpression->GetTokenType();
    if( !unaryOperatorMap.Contains( tokenType ) )
        ThrowError( "invalid unary operator : %c" , tokenType );

    auto opcode = unaryOperatorMap[ tokenType ];

    auto expression = unaryExpression->GetExpression();
    CompileExpressionAndProcessReturnValue( context , expression );
    u32 expressionIdx = context->GetLastTempVariableIndex();

    encodeHelper.EncodeDirectly( opcode,
                                 OperandType::TempVariable , expressionIdx ,
                                 OperandType::TempVariable , expressionIdx ,
                                 OperandType::None , 0
                                 );
}


void Compiler::CompileBinaryExpression( CompileContext *context , BinaryOperationExpression * binaryExpression )
{
    Encoder::Helper encodeHelper( context );
    static const UnorderedMap< s32 , Opcode > binaryOperatorMap = {

            {  '^', Opcode::BinaryOpPower } ,

            {  Token::TK_MOD , Opcode::BinaryOpModulo } ,
            {  Token::TK_IDIV , Opcode::BinaryOpIntDivide } ,
            {  '*' , Opcode::BinaryOpMultiply } ,
            {  '/' , Opcode::BinaryOpDivide } ,

            {  '+' , Opcode::BinaryOpAdd } ,
            {  '-' , Opcode::BinaryOpSubtract } ,

            {  Token::TK_CONCAT , Opcode::BinaryOpConcat } ,

            {  Token::TK_SHL , Opcode::BinaryOpShiftLeft } ,
            {  Token::TK_SHR , Opcode::BinaryOpShiftRight } ,

            {  '&' , Opcode::BinaryOpBitwiseAnd } ,
            {  '~', Opcode::BinaryOpBitwiseXor } ,
            {  '|' , Opcode::BinaryOpBitwiseOr } ,

            {  Token::TK_LESSEQUAL , Opcode::BinaryOpCmpLessThanOrEqual } ,
            {  '<', Opcode::BinaryOpCmpLessThan } ,
            {  Token::TK_EQUAL , Opcode::BinaryOpCmpEqual } ,
            {  Token::TK_NOTEQUAL , Opcode::BinaryOpCmpNotEqual } ,
            {  Token::TK_GREATEREQUAL , Opcode::BinaryOpCmpGreaterThanOrEqual } ,
            {  '>', Opcode::BinaryOpCmpGreaterThan } ,

            {  Token::TK_AND , Opcode::BinaryOpLogicalAnd } ,
            {  Token::TK_OR , Opcode::BinaryOpLogicalOr } ,
    };
    if( !binaryOperatorMap.Contains( binaryExpression->GetOperator() ) )
        ThrowError( "invalid binary operator : %d" , binaryExpression->GetOperator() );


    auto leftStatement = binaryExpression->GetLeft();
    auto rightStatement = binaryExpression->GetRight();
    if( auto leftBinaryExpression = LXX::Cast< BinaryOperationExpression >( leftStatement ) )
        CompileBinaryExpression( context , leftBinaryExpression );
    else
        CompileExpressionAndProcessReturnValue( context , leftStatement );

    u32 leftTempIdx = context->GetLastTempVariableIndex();

    if( auto rightBinaryExpression = LXX::Cast< BinaryOperationExpression >( rightStatement ) )
        CompileBinaryExpression( context , rightBinaryExpression );
    else
        CompileExpressionAndProcessReturnValue( context , rightStatement );

    u32 rightTempIdx = context->GetLastTempVariableIndex();
    u32 destTempIdx = context->AddTempVariable();

    encodeHelper.EncodeDirectly( binaryOperatorMap[ binaryExpression->GetOperator() ] ,
                        OperandType::TempVariable , destTempIdx ,
                        OperandType::TempVariable , leftTempIdx ,
                        OperandType::TempVariable , rightTempIdx );
}


void Compiler::CompileAssignmentStatement( CompileContext *context , AssignmentStatement * assignmentStatement )
{
    Encoder::Helper encodeHelper( context );
    ByteCodeChunk * chunk = context->GetLuaClosure()->GetChunk();

    auto varListExpression = LXX::Cast< VarListExpression >( assignmentStatement->GetLeft() );
    if( varListExpression == nullptr )
        ThrowError( "invalid assignment statement , except var expression list" );

    auto expressionList = LXX::Cast< ExpressionListStatement >( assignmentStatement->GetRight() );
    if( expressionList == nullptr )
        ThrowError( "invalid assignment statement , except expression list" );

    if( !IsComplicatedExpressionListStatement( expressionList ) )
    {
        if( varListExpression->GetVarExpressionCount() < expressionList->GetExpressionCount() )
            ThrowError( "invalid assignment statement , variable count is less than expression count" );

        Array< VarExpression * > &varExpressions = varListExpression->GetVarExpressions();
        Array< StatementBase * > &expressions = expressionList->GetExpressions();

        Array< u32 > ExpressionValueIdx;
        for( u32 iExpression = 0 ; iExpression < expressions.Size() ; iExpression++ )
        {
            CompileExpression( context , expressions[ iExpression ] );
            ExpressionValueIdx.Add( context->GetLastTempVariableIndex() );
        }

        for(  u32 iVar = 0 ; iVar < varExpressions.Size() ; iVar++ )
        {
            bool bIsTable;
            OperandType operandType;
            u32 operandIdx;
            CompileVarExpression( context , varExpressions[ iVar ] ,  operandType , operandIdx , bIsTable );
            if( !bIsTable )
            {
                encodeHelper.Assign( operandType , operandIdx ,
                                     OperandType::TempVariable , ExpressionValueIdx[ iVar ]
                                     );
            }
            else
            {
                u32 tableIdx = context->GetLastTempVariableIndex();

                encodeHelper.SetField( OperandType::TempVariable, tableIdx
                        , operandType , operandIdx
                        , OperandType::TempVariable , ExpressionValueIdx[ iVar ]
                        );
            }
        }
    }
    else
    {

        Array< VarExpression * > &varExpressions = varListExpression->GetVarExpressions();

        CompileComplicatedExpressionListStatement( context, expressionList );

        u32 testConditionIdx = context->AddTempVariable();
        for( u32 iVar = 0 ; iVar < varExpressions.Size() ; iVar++ )
        {
            bool bIsTable;
            OperandType operandType;
            u32 operandIdx;
            CompileVarExpression( context , varExpressions[ iVar ] ,  operandType , operandIdx , bIsTable );

            if( !bIsTable )
            {
                encodeHelper.EncodeSimpleIfStatement(
                        [&]( Encoder::Helper &helper )
                        {
                            helper.EncodeDirectly( Opcode::BinaryOpCmpLessThan,
                                                   OperandType::TempVariable , testConditionIdx ,
                                                   OperandType::Immediate , iVar,
                                                   OperandType::Stack , Encoder::MakeOperandIndex( -1 )
                            );
                        }
                        , OperandType::TempVariable , testConditionIdx ,
                        [&]( Encoder::Helper &helper )
                        {
                            encodeHelper.Assign( operandType , operandIdx ,
                                                 OperandType::Stack, Encoder::MakeOperandIndex( -2 - iVar ) );
                        },
                        [&]( Encoder::Helper &helper )
                        {
                            encodeHelper.Assign( operandType , operandIdx ,
                                                 OperandType::Constant , chunk->GetConstNilValueIndex() );
                        }
                );
            }
            else
            {
                u32 tableIdx = context->GetLastTempVariableIndex();

                encodeHelper.SetField( OperandType::TempVariable, tableIdx
                        , operandType , operandIdx
                        , OperandType::Stack, Encoder::MakeOperandIndex( -2 - iVar ) );
            }
        }

        encodeHelper.Pop( OperandType::Immediate , 1 );
        encodeHelper.Pop( OperandType::Stack , Encoder::MakeOperandIndex( -1 ) );
    }

}


void Compiler::CompileBlockStatement( CompileContext *context , BlockStatement * blockStatement )
{
    for( auto child : blockStatement->GetStatements() )
    {
        CompileStatement( context , child );
    }
}


void Compiler::CompileFunctionStatementIgnoreName( CompileContext *context , FunctionStatement * functionStatement )
{
    Encoder::Helper encodeHelper( context );

    auto newContext = new CompileContext( functionStatement , context );
    CompileStatement( newContext , functionStatement->GetFunctionBody() );

    ByteCodeChunk * chunk = context->GetLuaClosure()->GetChunk();
    u32 luaClosureIdx = chunk->AddConstValue( newContext->GetLuaClosure() );
    encodeHelper.Assign( OperandType::TempVariable , context->AddTempVariable()
                            ,OperandType::Constant , luaClosureIdx );
}


void Compiler::CompileFunctionStatementWithName( CompileContext *context , FunctionStatement * functionStatement )
{
    Encoder::Helper encodeHelper( context );
    ByteCodeChunk * chunk = context->GetLuaClosure()->GetChunk();

    if ( functionStatement->GetFunctionName() == nullptr )
        ThrowError( "Function name is null" );

    auto functionNameStatement = functionStatement->GetFunctionName()->Cast< FunctionNameStatement >();
    if(functionNameStatement == nullptr )
        ThrowError( "invalid local function name ");

    Array< IdentifierExpression * > nameList;
    for( auto name : functionNameStatement->GetNames() )
    {
        if ( auto identifierExpression = LXX::Cast< IdentifierExpression >( name ) )
        {
            nameList.Add( identifierExpression );
        }
        else
        {
            ThrowError( "invalid local function name" );
        }
    }

    if( nameList.Size() == 0 )
        ThrowError( "invalid local function name" );


    CompileFunctionStatementIgnoreName( context , functionStatement );
    u32 functionIdx = context->GetLastTempVariableIndex();

    bool isTable = nameList.Size() > 1;
    bool containSelf = functionNameStatement->GetLastConcatToken() == ':';
    if( isTable )
    {
        u32 tableIdx = context->AddTempVariable();
        for( u32 iName = 0 ; iName < nameList.Size() -1 ; iName++ )
        {
            encodeHelper.GetField( OperandType::TempVariable , tableIdx
                , OperandType::TempVariable , context->AddTempVariable()
                ,  OperandType::Constant , chunk->AddConstValue( nameList[iName]->GetIdentifier() )
            );
        }
        encodeHelper.SetField( OperandType::TempVariable , tableIdx
                , OperandType::Constant , chunk->AddConstValue( nameList.GetLast()->GetIdentifier() )
                , OperandType::TempVariable , functionIdx
        );
    }
    else
    {
        u32 varIdx = -1;
        OperandType varType = OperandType::UpValue;
        String *functionName = nameList.GetFirst()->GetIdentifier();
        context->GetVariableOperandInfo( functionName, varType , varIdx );

        encodeHelper.Assign( varType , varIdx ,
                        OperandType::TempVariable , functionIdx );
    }
}


void Compiler::CompileLocalStatement( CompileContext *context , LocalStatement * localStatement )
{
    Encoder::Helper encodeHelper( context );

    auto DeclareLocalVar = [&]( VarListExpression * varListExpression )
    {
        if( !varListExpression )
            ThrowError( "invalid local variable declaration , var expression list is null" );
        for( auto varExpression : varListExpression->GetVarExpressions() )
        {
            Array< StatementBase * > &statements = varExpression->GetStatements();
            if( statements.Size() != 1 )
                ThrowError( "invalid local variable declaration" );
            auto identifierExpression = LXX::Cast< IdentifierExpression >( statements[0] );
            if( identifierExpression == nullptr )
                ThrowError( "invalid local variable declaration" );

            if( !context->AddLocalVariable(identifierExpression->GetIdentifier()) )
                ThrowError( "redefinition of local variable" );
        }
    };

    StatementBase *child = localStatement->GetChild();
    if( auto assignmentStatement = LXX::Cast< AssignmentStatement >( child ) )
    {
        DeclareLocalVar( LXX::Cast< VarListExpression >( assignmentStatement->GetLeft() ) );

        CompileAssignmentStatement( context , assignmentStatement );
    }
    else if( auto varListExpression = LXX::Cast< VarListExpression >( child ) )
    {
        DeclareLocalVar( varListExpression );
    }
    else if( auto functionStatement = LXX::Cast< FunctionStatement >( child ) )
    {
        auto functionName = LXX::Cast< FunctionNameStatement >( functionStatement->GetFunctionName() );
        if( functionName == nullptr )
            ThrowError( "invalid local function name" );
        if( functionName->GetNames().Size() > 1 )
            ThrowError( "invalid local function name" );
        IdentifierExpression *nameExpression = LXX::Cast< IdentifierExpression >( functionName->GetNames().GetFirst() );
        if(  nameExpression == nullptr )
            ThrowError( "invalid local function name" );

        if( !context->AddLocalVariable( nameExpression->GetIdentifier() ) )
            ThrowError( "redefinition of local variable" );

        CompileFunctionStatementWithName( context , functionStatement );
    }
}

void Compiler::CompileStatement( CompileContext * context, StatementBase * statement )
{
    ByteCodeChunk *chunk = context->GetLuaClosure()->GetChunk();
    if( auto localStatement = LXX::Cast< LocalStatement >( statement ) )
    {
        CompileLocalStatement( context , localStatement );
    }
    else if( auto assignmentStatement = LXX::Cast< AssignmentStatement >( statement ) )
    {
        CompileAssignmentStatement( context , assignmentStatement );
    }
    else if( auto blockStatement = LXX::Cast< BlockStatement >( statement ) )
    {
        CompileBlockStatement( context , blockStatement );
    }
    else if( auto functionCallExpression = LXX::Cast< FunctionCallExpression >( statement ) )
    {
        Encoder::Helper encodeHelper( context );
        u32 argumentNumIdx;
        CompileFunctionCallExpression( context , functionCallExpression , argumentNumIdx );
        encodeHelper.Pop( OperandType::TempVariable , context->GetLastTempVariableIndex() ); ;

        // pop all argument and function
        encodeHelper.Increase(
                OperandType::TempVariable , argumentNumIdx,
                1
                );
        encodeHelper.Pop( OperandType::TempVariable , argumentNumIdx );

    }
    else if( auto whileStatement = LXX::Cast< WhileStatement >( statement ) )
    {
        CompileWhileStatement( context , whileStatement );
    }
    else if( auto continueStatement = LXX::Cast< ContinueStatement >( statement ) )
    {
        CompileContinueStatement( context , continueStatement );
    }
    else if( auto breakStatement = LXX::Cast< BreakStatement >( statement ) )
    {
        CompileBreakStatement( context , breakStatement );
    }
    else if( auto repeatStatement = LXX::Cast< RepeatStatement >( statement ) )
    {
        CompileRepeatStatement( context , repeatStatement );
    }
    else if( auto ifStatement = LXX::Cast< IfStatement >( statement ) )
    {
        CompileIfStatement( context , ifStatement );
    }
    else if( auto forLoopStatement = LXX::Cast< ForLoopStatement >( statement ) )
    {
        CompileForLoopStatement( context , forLoopStatement );
    }
    else if( auto returnStatement = LXX::Cast< ReturnStatement >( statement ) )
    {
        CompileReturnStatement( context , returnStatement );
    }
    else if( auto functionStatement = LXX::Cast< FunctionStatement >( statement ) )
    {
        CompileFunctionStatementWithName( context , functionStatement );
    }
    else
    {
        ThrowError( "not implemented" );
    }
}


LuaClosure* Compiler::CompileStatement( StatementBase* statement )
{
    if( statement == nullptr ||  statement->IsA< FunctionStatement >() == false )
        ThrowError( "invalid function statement" );

    auto functionStatement = LXX::Cast< FunctionStatement >( statement );
    if( functionStatement == nullptr )
        ThrowError( "invalid function statement , function statement is null" );

    auto functionBody = LXX::Cast< BlockStatement >( functionStatement->GetFunctionBody() );
    if( functionBody == nullptr )
        ThrowError( "invalid function statement , function body is null" );


    CompileContext * context = new CompileContext( functionStatement );

    for( auto child : functionBody->GetStatements() )
    {
        context->ResetLastTempVariableIndex();
        CompileStatement( context , child );
    }

    return context->GetLuaClosure();
}


} // LXX