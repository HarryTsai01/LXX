//
// Created by Harry on 8/10/2023.
//

#ifndef XLUA_PARSER_H
#define XLUA_PARSER_H
#include "Lexer.h"
#include "AST/FunctionStatement.h"
#include <core/objects/GCObject.h>
#include <core/objects/GCObjectCollector.h>
#include <core/compiler/Ast/Expressions/ExpressionStatementBase.h>
#include <core/Containers/Array.h>

namespace LXX {


class FunctionStatement;

class Parser : public GCObject
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    Parser();
    ~Parser();

    void SetLexer(Lexer* pLexer) { m_pLexer = pLexer; }
    FunctionStatement* CompileLuaClosure( );

    virtual void CollectReferences( GCObjectCollector& collector ) ;

private:
    void ThrowError( const char* message );
    StatementBase* ParseTableConstructor();
    StatementBase* ParseFunctionDefinitionWithName();
    StatementBase* ParseFunctionDefinition();
    void ParseFunctionParameterList( class FunctionStatement* pFunction );
    StatementBase* ParseFunctionName();
    StatementBase* ParseFunctionCall( StatementBase* varExpression , StatementBase*  functionName );
    StatementBase* ParseBlock();
    StatementBase* ParseStatement();
    StatementBase* ParseAssignmentStatement( StatementBase* varExpressionList );
    StatementBase* ParseVarListExpression();
    StatementBase* ParseLocalState();
    StatementBase* ParseBreak();
    StatementBase* ParseDoBlockEnd();
    StatementBase* ParseWhile();
    StatementBase* ParseRepeat();
    StatementBase* ParseIf();
    StatementBase* ParseFor();
    StatementBase* ParseExpressionList();
    StatementBase* ParseExpression();
    StatementBase* ParseSimpleExpression();
    StatementBase* ParsePrefixExpression();
    StatementBase* ParseVarExpression();
    StatementBase* ParseReturn();
    StatementBase* ParseContinue();
    void CheckFunctionParameters( FunctionStatement* functionStatement );
private:
    Lexer* m_pLexer;
};


} // LXX

#endif //XLUA_PARSER_H
