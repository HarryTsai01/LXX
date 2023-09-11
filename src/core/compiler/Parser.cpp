//
// Created by Harry on 8/10/2023.
//

#include "Parser.h"
#include "LexerUtil.h"
#include <core/objects/string/String.h>
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


Parser::Parser()
    :  m_pLexer( nullptr )
{

}


Parser::~Parser()
{

}


FunctionStatement* Parser::CompileLuaClosure( )
{
    FunctionStatement *functionStatement = new FunctionStatement();
    functionStatement->SetFunctionBody(ParseBlock() );
    return functionStatement;
}


void Parser::CollectReferences( GCObjectCollector &collector )
{
    collector.AddObject( m_pLexer );
}


StatementBase* Parser::ParseBlock()
{
    BlockStatement *statementList = new BlockStatement();
    CompatToken token;
    while( m_pLexer->LookAheadToken( token ) )
    {
        auto curStatement = ParseStatement();
        if( curStatement )
        {
            statementList->AddStatement( curStatement );

            if ( curStatement->IsA<ReturnStatement>( ) )
                break;
        }
        else
        {
            break;
        }
    }
    return statementList;
}


StatementBase* Parser::ParseAssignmentStatement( StatementBase* varExpressionList )
{
    CompatToken token;
    if( !m_pLexer->NextTokenIf( token , '=' , Token::TK_IN ) )
        ThrowError( " Invalid assignment statement , expect '=' or 'in' " );

    AssignmentStatement *statement = new AssignmentStatement();
    statement->SetOperators(
            varExpressionList ,
            ParseExpressionList()
    );
    return statement;
}


StatementBase* Parser::ParseStatement()
{
    // comma
    CompatToken token;
    StatementBase * statement = nullptr;
    if( m_pLexer->LookAheadToken( token ) )
    {
        if ( token.GetTokenType() == Token::TK_LOCAL )
        {
            statement = ParseLocalState() ;
        }
        else if ( token.GetTokenType() == Token::TK_BREAK )
        {
            statement = ParseBreak();
        }
        else if ( token.GetTokenType() == Token::TK_DO )
        {
            statement = ParseDoBlockEnd();
        }
        else if ( token.GetTokenType() == Token::TK_WHILE )
        {
            statement = ParseWhile();
        }
        else if ( token.GetTokenType() == Token::TK_REPEAT )
        {
            statement = ParseRepeat();
        }
        else if ( token.GetTokenType() == Token::TK_IF )
        {
            statement = ParseIf();
        }
        else if ( token.GetTokenType() == Token::TK_FOR )
        {
            statement = ParseFor();
        }
        else if ( token.GetTokenType() == Token::TK_FUNCTION )
        {
            statement = ParseFunctionDefinitionWithName();
        }
        else if( token.GetTokenType() == Token::TK_RETURN )
        {
            statement = ParseReturn();
        }
        else if( token.GetTokenType() == Token::TK_CONTINUE )
        {
            statement = ParseContinue();
        }
        else if( token.GetTokenType() == ';' )
        {
            statement = new SemicolonStatement();
        }
        else if( token.GetTokenType() == Token::TK_IDENTIFY )
        {
            VarListExpression *varListExpression = LXX::Cast< VarListExpression >( ParseVarListExpression() );
            if( varListExpression->GetVarExpressionCount() == 1
                && !m_pLexer->LookAheadTokenIf( token , '=' ) )
            {
                StatementBase *functionName = nullptr;
                if( m_pLexer->NextTokenIf( token  , ':' ) )
                {
                    if( !m_pLexer->NextTokenIf( token , Token::TK_IDENTIFY ) )
                        ThrowError( "Invalid statement , function name expected" );
                    functionName = new IdentifierExpression( NEW_STRING( token.AsIdentifier() ) );

                }
                statement = ParseFunctionCall( varListExpression->GetVarExpressions()[0] , functionName );
            }
            else
            {
                statement = ParseAssignmentStatement( varListExpression );
            }
        }
    }

    return statement;
}


StatementBase* Parser::ParseLocalState()
{
    CompatToken token;
    if(  !m_pLexer->NextTokenIf( token  , Token::TK_LOCAL ) )
        ThrowError( "Invalid local statement" );

    LocalStatement * localStatement = new LocalStatement();

    if( m_pLexer->LookAheadTokenIf( token  , Token::TK_FUNCTION  , Token::TK_IDENTIFY ) )
    {
        if( token.GetTokenType() == Token::TK_FUNCTION )
        {
            localStatement->SetChild( ParseFunctionDefinitionWithName() );
        }
        else
        {
            auto varListExpression = ParseVarListExpression();
            if ( m_pLexer->LookAheadTokenIf( token  , '=' ) )
            {
                localStatement->SetChild( ParseAssignmentStatement( varListExpression ) );
            }
            else
            {
                localStatement->SetChild( varListExpression );
            }
        }
    }
    else
    {
        ThrowError( "Invalid local statement , expected 'function' or 'identifier'" );
    }

    return localStatement;
}


StatementBase* Parser::ParseBreak()
{
    StatementBase * statement = nullptr;
    CompatToken token;
    if( !m_pLexer->NextTokenIf( token  , Token::TK_BREAK ) )
        ThrowError( "Invalid break, expected 'break'" );
    statement = new BreakStatement();
    return statement;
}


StatementBase* Parser::ParseDoBlockEnd()
{
    StatementBase * statement = nullptr;
    CompatToken token;
    if( !m_pLexer->NextTokenIf( token  , Token::TK_DO ) )
        ThrowError( "Invalid do, expected 'do'" );

    statement = ParseBlock();

    if( !m_pLexer->NextTokenIf( token  , Token::TK_END ) )
        ThrowError( "Invalid end, expected 'end'" );

    return statement;
}


StatementBase* Parser::ParseWhile()
{
    StatementBase * statement = nullptr;
    CompatToken token;
    if( !m_pLexer->NextTokenIf( token  , Token::TK_WHILE ) )
        ThrowError( "Invalid while, expected 'while'" );

    auto whileStatement = new WhileStatement();
    whileStatement->SetCondition( ParseExpression() );
    whileStatement->SetBody( ParseDoBlockEnd() );

    return whileStatement;
}


StatementBase* Parser::ParseRepeat()
{
    StatementBase * statement = nullptr;
    CompatToken token;
    if( !m_pLexer->NextTokenIf( token  , Token::TK_REPEAT ) )
        ThrowError( "Invalid repeat, expected 'repeat'" );

    auto repeatStatement = new RepeatStatement();
    repeatStatement->SetBody( ParseBlock() );

    if( !m_pLexer->NextTokenIf( token  , Token::TK_UNTIL ) )
        ThrowError( "Invalid until, expected 'until'" );

    repeatStatement->SetCondition( ParseExpression() );

    return repeatStatement;
}


StatementBase* Parser::ParseIf()
{
    CompatToken token;
    if( !m_pLexer->NextTokenIf( token  , Token::TK_IF ) )
        ThrowError( "Invalid if, expected 'if'" );

    auto ifStatement = new IfStatement();
    ifStatement->SetCondition( ParseExpression() );

    if( !m_pLexer->NextTokenIf( token  , Token::TK_THEN ) )
        ThrowError( "Invalid if statement, expected 'then'" );

    ifStatement->SetThen( ParseBlock() );

    while( m_pLexer->NextTokenIf( token  , Token::TK_ELSEIF ) )
    {
        auto elseIfStatement = new IfStatement();
        elseIfStatement->SetCondition( ParseExpression() );

        if( !m_pLexer->NextTokenIf( token  , Token::TK_THEN ) )
            ThrowError( "Invalid elseif statement, expected 'then'" );

        elseIfStatement->SetThen( ParseBlock() );

        ifStatement->AddElseif( elseIfStatement );
    }

    if( m_pLexer->NextTokenIf( token  , Token::TK_ELSE ) )
        ifStatement->SetElse( ParseBlock() );

    if(  !m_pLexer->NextTokenIf( token  , Token::TK_END ) )
        ThrowError( "Invalid if statement, expected 'end'" );

    return ifStatement;
}


StatementBase* Parser::ParseFor()
{
    CompatToken token;
    if( !m_pLexer->NextTokenIf( token  , Token::TK_FOR ) )
        ThrowError( "Invalid for, expected 'for'" );

    auto forLoopStatement = new ForLoopStatement();
    forLoopStatement->SetCondition( ParseAssignmentStatement( ParseVarListExpression() ) );
    forLoopStatement->SetBody( ParseDoBlockEnd() );

    return forLoopStatement;
}


StatementBase* Parser::ParseTableConstructor()
{
    CompatToken token;
    if( !m_pLexer->NextTokenIf( token, '{' ) )
        ThrowError( "Invalid table constructor, expected '{'" );

    TableConstructorStatement * tableConstructorStatement = new TableConstructorStatement();

    while( !m_pLexer->LookAheadTokenIf( token, '}' ) )
    {
        TableFieldStatement * tableFieldStatement = new TableFieldStatement();
        if( m_pLexer->NextTokenIf( token, Token::TK_IDENTIFY ) )
        {
            auto identifierExpression = new IdentifierExpression( NEW_STRING( token.AsIdentifier() ) );
            // Name ‘=’ exp | Name ,
            if( m_pLexer->NextTokenIf( token, &LexerUtil::IsTableFieldSeparator ) )
            {
                tableFieldStatement->SetValue( identifierExpression );
            }
            else if( m_pLexer->NextTokenIf( token, '=' ) )
            {
                tableFieldStatement->SetKey( identifierExpression );
                tableFieldStatement->SetValue( ParseExpression() );
            }
            else
            {
                ThrowError( "Invalid table constructor, invalid token" );
            }
        }
        else if( m_pLexer->NextTokenIf( token, '[' ) )
        {
            tableFieldStatement->SetKey( ParseExpression() );

            if( !m_pLexer->NextTokenIf( token, ']' ))
                ThrowError( "Invalid table constructor, expected ']'" );

            if( !m_pLexer->NextTokenIf( token, '=' ))
                ThrowError( "Invalid table constructor, expected '='" );

            tableFieldStatement->SetValue( ParseExpression() );
        }
        else
        {
            tableFieldStatement->SetValue( ParseExpression() );
        }

        tableConstructorStatement->AddField( tableFieldStatement );

        // if this is the last field , the field separator is not required
        // BUT if it is not the last field , the field separator is required
        if( !m_pLexer->NextTokenIf( token, &LexerUtil::IsTableFieldSeparator ) )
        {
            if( !m_pLexer->LookAheadTokenIf( token, '}' ) )
                ThrowError( "Invalid table constructor, expected ',' or ';' as the field separator" );
        }
    }

    if( !m_pLexer->NextTokenIf( token, '}' ) )
        ThrowError( "Invalid table constructor, expected '}'" );
    return tableConstructorStatement;
}


StatementBase* Parser::ParseVarListExpression()
{
    VarExpression *varExpression = LXX::Cast< VarExpression >(ParseVarExpression() ) ;
    VarListExpression *varListExpression = new VarListExpression();
    varListExpression->AddVarExpression( varExpression );
    CompatToken token;
    while( m_pLexer->NextTokenIf( token  , ',' ) )
    {
        varListExpression->AddVarExpression(LXX::Cast< VarExpression >(ParseVarExpression() ) );
    }

    return varListExpression;
}


StatementBase* Parser::ParseFunctionDefinitionWithName()
{
    CompatToken token;
    if( !m_pLexer->NextTokenIf( token  , Token::TK_FUNCTION ) )
        ThrowError( "Invalid function definition, expected 'function' " );

    FunctionStatement * functionStatement = new FunctionStatement();
    FunctionNameStatement * functionNameStatement = LXX::Cast< FunctionNameStatement >( ParseFunctionName() );
    functionStatement->SetFunctionName( functionNameStatement );

    ParseFunctionParameterList( functionStatement );

    if( functionNameStatement->GetLastConcatToken() == ':' )
    {
        functionStatement->AddSelfParameter( ) ;
    }

    CheckFunctionParameters(  functionStatement );

    functionStatement->SetFunctionBody( ParseBlock() );

    if( !m_pLexer->NextTokenIf( token  , Token::TK_END ) )
        ThrowError( "Invalid function definition, expected 'end' " );

    return functionStatement;
}


void Parser::ParseFunctionParameterList( FunctionStatement* pFunction  )
{
    /*
     * ‘(’ [parlist] ‘)’
     * parlist ::= namelist [‘,’ ‘...’] | ‘...’
     * namelist ::= Name {‘,’ Name}
     * */

    CompatToken token;
    if( !m_pLexer->NextTokenIf( token  , '(' ) )
        ThrowError( "Invalid function parameter list, expected '('" );

    while( m_pLexer->NextTokenIf( token, Token::TK_IDENTIFY  , Token::TK_DOT ) )
    {
        if ( token.GetTokenType() == Token::TK_IDENTIFY )
        {
            pFunction->AddParameter( new IdentifierExpression( NEW_STRING( token.AsIdentifier() ) ) );
        }
        else
        {
            pFunction->AddParameter( new IdentifierExpression( NEW_STRING("...") ) );
            break;
        }

        if( !m_pLexer->NextTokenIf( token, ',' ) ) break;
    }

    if( !m_pLexer->NextTokenIf( token  , ')' ) )
        ThrowError( "Invalid function parameter list, expected ')'" );

}


StatementBase* Parser::ParseFunctionName()
{
    // funcname ::= Name {‘.’ Name} [‘:’ Name]
    CompatToken token;
    if( !m_pLexer->NextTokenIf( token  , Token::TK_IDENTIFY ) )
        ThrowError( "Invalid function name, expected 'identifier'" );

    FunctionNameStatement *statement = new  FunctionNameStatement();
    statement->AddName( new IdentifierExpression( NEW_STRING( token.AsIdentifier() ) ) );

    while( m_pLexer->NextTokenIf( token  , '.' , ':') )
    {
        s32 lastConcatToken = token.GetTokenType();

        if( !m_pLexer->NextTokenIf( token  , Token::TK_IDENTIFY ) )
            ThrowError( "Invalid function name, expected 'identifier'" );

        statement->AddName( new IdentifierExpression( NEW_STRING( token.AsIdentifier() ) ) );
        statement->SetLastConcatToken( lastConcatToken );
    }

    return statement;
}

StatementBase* Parser::ParseFunctionDefinition()
{
    CompatToken token;
    if( !m_pLexer->NextTokenIf( token  , Token::TK_FUNCTION ) )
        ThrowError( "Invalid function definition, expected 'function' " );

    FunctionStatement * functionStatement = new FunctionStatement();
    ParseFunctionParameterList( functionStatement );

    CheckFunctionParameters(  functionStatement );

    functionStatement->SetFunctionBody( ParseBlock() );

    if( !m_pLexer->NextTokenIf( token  , Token::TK_END ) )
        ThrowError( "Invalid function definition, expected 'end' " );


    return functionStatement;
}


StatementBase* Parser::ParseFunctionCall( StatementBase* varExpression , StatementBase* functionName )
{
    FunctionCallExpression * functionCallExpression = new FunctionCallExpression();
    functionCallExpression->SetVarExpression( varExpression );
    functionCallExpression->SetFunctionName( functionName );

    CompatToken token;
    if( m_pLexer->NextTokenIf( token , '(') )
    {
        if ( !m_pLexer->NextTokenIf( token , ')' ) )
        {
            functionCallExpression->SetArguments( ParseExpressionList() );

            if( !m_pLexer->NextTokenIf( token , ')' ) )
                ThrowError( "Invalid function call , Expected ')'" );
        }
    }
    else if ( m_pLexer->NextTokenIf( token , Token::TK_STRING ) )
    {
        functionCallExpression->SetArguments( new StringExpression(NEW_STRING(token.AsString() ) ) );
    }
    else if( m_pLexer->LookAheadTokenIf( token , '{' ) )
    {
        functionCallExpression->SetArguments( ParseTableConstructor() );
    }
    else
    {
        ThrowError( "Invalid function call " );
    }

    return functionCallExpression;
}


StatementBase* Parser::ParseExpressionList()
{
    CompatToken token;
    auto expressionList = new ExpressionListStatement();
    do
    {
        expressionList->AddExpression( ParseExpression() );
    }
    while ( m_pLexer->NextTokenIf( token , ',' ) );

    return expressionList;
}


StatementBase* Parser::ParseSimpleExpression()
{
    CompatToken token;
    if ( m_pLexer->NextTokenIf( token , Token::TK_NIL ) )
    {
        return new NilExpression();
    }
    else if ( m_pLexer->NextTokenIf( token , Token::TK_FALSE ,  Token::TK_TRUE ) )
    {
        return new BooleanExpression( token.GetTokenType() == Token::TK_TRUE );
    }
    else if ( m_pLexer->NextTokenIf( token , Token::TK_INT ) )
    {
        return new IntegerNumberExpression(token.AsInteger() );
    }
    else if ( m_pLexer->NextTokenIf( token , Token::TK_REAL ) )
    {
        return new RealNumberExpression( token.AsReal() );
    }
    else if ( m_pLexer->NextTokenIf( token , Token::TK_STRING ) )
    {
        return new StringExpression(NEW_STRING(token.AsString() ) );
    }
    else if ( m_pLexer->NextTokenIf( token , Token::TK_DOT ) )
    {
        return new DotExpression();
    }
    else if ( m_pLexer->LookAheadTokenIf( token , Token::TK_FUNCTION ))
    {
        return ParseFunctionDefinition();
    }
    else if ( m_pLexer->LookAheadTokenIf( token , '{' ) )
    {
        return ParseTableConstructor();
    }
    else
    {
        return ParsePrefixExpression();
    }
}


StatementBase* Parser::ParseExpression()
{
    StatementBase * statement = nullptr;

    CompatToken token;
    if( m_pLexer->NextTokenIf( token , & LexerUtil::IsUnaryOp ) )
    {
        statement = new UnaryOperatorExpression( token.GetTokenType() , ParseExpression() );
    }
    else
    {
        static const UnorderedMap< s32 , s32 > precedence = {
                { Token::TK_OR , 1 },
                { Token::TK_AND , 2 },
                { '<' , 3 },
                { '>' , 3 },
                { Token::TK_LESSEQUAL , 3 },
                { Token::TK_GREATEREQUAL , 3 },
                { Token::TK_EQUAL , 3 },
                { Token::TK_NOTEQUAL , 3 },
                { '|' , 4 },
                { '~' , 5 },
                { '&' , 6 },
                { Token::TK_SHL , 7 },
                { Token::TK_SHR , 7 },
                { Token::TK_CONCAT , 8 },
                { '+' , 9 },
                { '-' , 9 },
                { '*' , 10 },
                { '/' , 10 },
                { Token::TK_MOD , 10 } ,
                { Token::TK_IDIV , 10 } ,
                { '^' , 11 },
        };
        Array< s32 > binaryOperators;
        Array< StatementBase* > operators = { ParseSimpleExpression() };
        while( m_pLexer->NextTokenIf( token ,& LexerUtil::IsBinaryOp  ) )
        {
            operators.PushBack( ParseSimpleExpression() );
            binaryOperators.PushBack( token.GetTokenType() );
        }
        if( binaryOperators.Size() > 1 )
        {
            while( operators.Size() > 1 )
            {
                u32 highestPrecedenceIdx = 0;
                u32 highestPrecedence = 0;
                for( u32 i = 1 ; i < binaryOperators.Size() ; i++ )
                {
                    if( highestPrecedence < precedence[ binaryOperators[i] ] )
                    {
                        highestPrecedence = precedence[ binaryOperators[i] ];
                        highestPrecedenceIdx = i;
                    }
                }

                auto newExpression = new BinaryOperationExpression( binaryOperators[highestPrecedenceIdx] ,
                                                           operators[highestPrecedenceIdx] ,
                                                           operators[highestPrecedenceIdx+1] );

                operators.RemoveAt( highestPrecedenceIdx );
                operators[ highestPrecedenceIdx ] = newExpression;
                binaryOperators.RemoveAt( highestPrecedenceIdx );
            }

            assert( operators.Size() == 1 );
            statement = operators[0];
        }
        else
        {
            statement = binaryOperators.Size() == 0 ? operators[0] :
                    new BinaryOperationExpression( binaryOperators[0] ,
                                                   operators[0] ,
                                                   operators[1] );
        }
    }

    return statement;
}


StatementBase* Parser::ParsePrefixExpression()
{
    /*
     * prefixexp ::= var | functioncall | ‘(’ exp ‘)’
     * functioncall ::=  var args | var ‘:’ Name args
     * args ::=  ‘(’ [explist] ‘)’ | tableconstructor | LiteralString
     * var ::=  Name | prefixexp ‘[’ exp ‘]’ | prefixexp ‘.’ Name
     * */
    StatementBase * statement = nullptr;
    CompatToken token;
    if( m_pLexer->NextTokenIf( token , '(' ) )
    {
        statement = ParseExpression();
        if( m_pLexer->NextTokenIf( token  , ')' ) )
            ThrowError( "Expected ')' after expression" );
    }
    else
    {
        auto varExpression = ParseVarExpression();
        StatementBase *functionName = nullptr;
        if( m_pLexer->NextTokenIf( token  , ':' ) )
        {
            if( !m_pLexer->NextTokenIf( token , Token::TK_IDENTIFY ) )
                functionName = new IdentifierExpression( NEW_STRING( token.AsIdentifier() ) );
        }

        if ( m_pLexer->LookAheadTokenIf( token ,
                                          '(' ,
                                          Token::TK_STRING ,
                                          '{' ) )
        {
            statement = ParseFunctionCall( varExpression , functionName );
        }
        else
        {
            statement = varExpression;
        }
    }

    return statement;
}


StatementBase* Parser::ParseVarExpression()
{
    /*
     * var ::=  Name | var ‘[’ exp ‘]’ | var ‘.’ Name
     * */
    CompatToken token;
    if ( !m_pLexer->NextTokenIf( token , Token::TK_IDENTIFY ) )
        ThrowError( "Expected identifier" );

    auto newVarExpression = new VarExpression( new IdentifierExpression( NEW_STRING( token.AsIdentifier() ) ) );

    while ( m_pLexer->NextTokenIf( token , '.' , '[') )
    {
        if ( token.GetTokenType() == '.' )
        {
            if ( !m_pLexer->NextTokenIf( token , Token::TK_IDENTIFY ) )
                ThrowError( "Expected identifier" );

            newVarExpression->AddStatement(new IdentifierExpression(NEW_STRING( token.AsIdentifier() ) ) );
        }
        else
        {
            newVarExpression->AddStatement(ParseExpression() );
            m_pLexer->NextToken( token );
            if ( token.GetTokenType() != ']' )
                ThrowError( "Expected ']' after identifier" );
        }
    }

    return newVarExpression;
}


StatementBase* Parser::ParseReturn()
{
    /*
     * retstat ::= return [explist] [‘;’]
     * */
    CompatToken token;
    if( !m_pLexer->NextTokenIf( token ,  Token::TK_RETURN ) )
        ThrowError( "Expected 'return'" );

    ReturnStatement * returnStatement = new ReturnStatement();

    /*
     * the expression list is optional
     * it is difficult to tell whether the return statement contains any value or not.
     *  From the construction of any expression list below , it is difficult to check the following token stream can
     *  make a return statement or not , because of there are too many situations we need to check.
     *  explist ::= exp {‘,’ exp}
     *  exp ::=  simpleexp | simpleexp binop simpleexp | unop exp
     *  simpleexp ::=  nil | false | true | Numeral | LiteralString | ‘...’ | functiondef |
        prefixexp | tableconstructor

        For simplification , there are two things we need to here
            1. Save the current position of the lexer
            2. Try to parse the expression list and if it fails , we can just restore the current position and go on.
     * when we try to parse the optional expression list , and if the expression list is not desired,
     *  it would fail to parse and throw compile error exception.
     *  so we need to save the current position and try to parse the expression list.
     *  when a CompileString error exception is thrown, we need to restore the current position from saved position.
     * */
    StatementBase * returnVal = nullptr;
    m_pLexer->SaveCurrentPosition();
    try
    {
        returnVal = ParseExpressionList();
    } catch ( ... )
    {
        m_pLexer->RestoreCurrentPosition();
    }

    returnStatement->SetReturnValue( returnVal );

    // skip ';'
    m_pLexer->NextTokenIf( token , ';' );

    return returnStatement;
}


StatementBase* Parser::ParseContinue()
{
    CompatToken token;
    if( !m_pLexer->NextTokenIf( token ,  Token::TK_CONTINUE ) )
        ThrowError( "Expected 'continue'" );

    ContinueStatement * continueStatement = new ContinueStatement();
    return continueStatement;
}


void Parser::CheckFunctionParameters( FunctionStatement* functionStatement )
{
    Array< StatementBase* > parameters = functionStatement->GetParameters();
    Array< String * > parameterNames;
    for( auto parameter : parameters )
    {
        if( auto identifierExpression = LXX::Cast< IdentifierExpression >( parameter ) )
        {
            parameterNames.PushBack( identifierExpression->GetIdentifier() );
        }
    }

    for( u32 iParameter = 0 ; iParameter < parameters.Size() ; iParameter++ )
    {
        String* curParameterName = parameterNames[ iParameter ];
        for( u32 jParameter = 0 ; jParameter < parameters.Size() ; jParameter++ )
        {
            if( iParameter != jParameter && curParameterName->Compare( *parameterNames[ jParameter ] ) == 0 )
            {
                ThrowError( "Duplicate parameter name" );
            }
        }
    }
}

void Parser::ThrowError( const char* message )
{
    m_pLexer->ThrowError( message );
}


} // LXX