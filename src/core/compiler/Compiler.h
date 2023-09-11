//
// Created by Harry on 8/10/2023.
//

#ifndef XLUA_COMPILER_H
#define XLUA_COMPILER_H
#include <string>
#include <core/objects/GCObject.h>
#include <core/objects/LuaClosure.h>
#include <core/objects/String/String.h>
#include <core/vm/Instructions/InstructionSet.h>
#include <core/exceptions/ExceptionCompileError.h>
#include "Lexer.h"
#include "Parser.h"

namespace LXX
{

class ByteCodeChunk;
class LocalStatement;
class AssignmentStatement;
class FunctionStatement;
class BlockStatement;
class BinaryOperationExpression;
class UnaryOperatorExpression;
class TableConstructorStatement;
class FunctionCallExpression;
class VarExpression;
class WhileStatement;
class RepeatStatement;
class ContinueStatement;
class BreakStatement;
class IfStatement;
class ForLoopStatement;
class ReturnStatement;
class IdentifierExpression;
class ExpressionListStatement;
class DotExpression;

using namespace Instruction;

class CompileContext : public GCObject
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    CompileContext( FunctionStatement *functionStatement , CompileContext *parent = nullptr );
    bool AddLocalVariable( String *name , u32* outIdx = nullptr );
    u32 AddTempVariable();
    u32 GetLastTempVariableIndex() { return _luaClosure->GetChunk()->GetLastTempVariableIndex(); }
    void GetVariableOperandInfo( String *variableName , OperandType &operandType , u32 &operandIndex );

    CompileContext* GetParent() { return _parent; }
    LuaClosure* GetLuaClosure() { return _luaClosure; }
    ByteCodeChunk* GetByteCodeChunk() { return _luaClosure->GetChunk(); }

    void AddBreakLocation( u32 location );
    void AddContinueLocation( u32 location );
    void RetrieveBreakLocation( Array< u32 > &locations  , u32 beginLocation , u32 endLocation );
    void RetrieveContinueLocation( Array< u32 > &locations  , u32 beginLocation , u32 endLocation );

    typedef void (CompileContext::*RetrievePlaceholderLocationFunction)( Array< u32 > &locations  , u32 beginLocation , u32 endLocation );
private:
    bool IsLocalVariable( String *name , u32 *outIdx = nullptr );
    bool IsParameterVariable( String *name , u32 *outIdx = nullptr );
    void RetrieveLocation( const Array< u32 > &locations , u32 beginLocation , u32 endLocation , Array< u32 > &outLocations );

private:
    FunctionStatement* _functionStatement;
    CompileContext *_parent;
    LuaClosure*  _luaClosure;
    Array< u32 > _breakStatementLocations;
    Array< u32 > _continueStatementLocations;
};

class Compiler : public GCObject
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    Compiler();
    ~Compiler();

    LuaClosure* CompileFile( String *scriptFileName );
    LuaClosure* CompileString( const char* scriptContent );

    virtual void CollectReferences( GCObjectCollector& collector );
private:
    void ThrowError( const char* message );
    template <typename ... Args>
    void ThrowError( const char *format, Args ... args )
    {
        throw ExceptionCompileError(
                    String::Format( format, std::forward<Args>( args )... )
                                     ,  scriptFileName , -1 );
    }
    String* LoadScriptContent( String* scriptFileName );
    StatementBase* CompileAST(String* scriptContent );
    LuaClosure* CompileStringImpl( String* scriptContent );
    LuaClosure* CompileStatement( StatementBase* statement );
    void CompileStatement( CompileContext *context , StatementBase * statement );
    void CompileLocalStatement( CompileContext *context , LocalStatement * localStatement );
    void CompileAssignmentStatement( CompileContext *context , AssignmentStatement * assignmentStatement );
    void CompileFunctionStatementIgnoreName( CompileContext *context , FunctionStatement * functionStatement );
    void CompileFunctionStatementWithName(CompileContext *context , FunctionStatement * functionStatement );
    void CompileBlockStatement( CompileContext *context , BlockStatement * blockStatement );
    void CompileExpression( CompileContext *context , StatementBase * statement );
    void CompileExpressionAndProcessReturnValue( CompileContext *context , StatementBase * statement );
    void CompileUnaryExpression( CompileContext *context , UnaryOperatorExpression * unaryExpression );
    void CompileBinaryExpression( CompileContext *context , BinaryOperationExpression * binaryExpression );
    void CompileTableConstructorStatement( CompileContext *context , TableConstructorStatement * tableConstructorStatement );
    void CompileFunctionCallExpression( CompileContext *context , FunctionCallExpression * functionCallExpression );
    void CompileVarExpression( CompileContext *context , VarExpression * varExpression , OperandType &operandType  , u32 &operandIndex , bool &isTable );
    void CompileWhileStatement( CompileContext *context , WhileStatement * whileStatement );
    void CompileContinueStatement( CompileContext *context , ContinueStatement * continueStatement );
    void CompileBreakStatement( CompileContext *context , BreakStatement * breakStatement );
    void CompileRepeatStatement( CompileContext *context , RepeatStatement * repeatStatement );
    void CompileIfStatement( CompileContext *context , IfStatement * ifStatement );
    void CompileForLoopStatement( CompileContext *context , ForLoopStatement * forLoopStatement );
    void CompileReturnStatement( CompileContext *context , ReturnStatement * returnStatement );
    void CompileIdentifierExpression( CompileContext *context , IdentifierExpression * identifierExpression );
    void CompileExpressionListStatement( CompileContext *context , ExpressionListStatement * expressionListStatement );
    void CompileDotExpression( CompileContext *context , DotExpression * dotExpression );
private:
    String *scriptFileName;
    Lexer* lexer;
    Parser* parser;
};


} // LXX

#endif //XLUA_COMPILER_H