//
// Created by Harry on 8/16/2023.
//

#include "LexerUtil.h"
#include "Token.h"
#include <cstring>
#include <core/Utilities.h>

namespace LXX
{


bool LexerUtil::IsReverseToken( const char *tokenBeg, const char *tokenEnd, s32 &outTokenType )
{
    struct ReverseTokenInfo
    {
        const char *token;
        u32 length;
        s32 tokenType;
    };

    static const ReverseTokenInfo reverseTokenInfo[] =
    {
            { "local",5, Token::TK_LOCAL },
            { "nil",3, Token::TK_NIL },
            { "true",4, Token::TK_TRUE },
            { "false",5, Token::TK_FALSE },
            { "end", 3,Token::TK_END },
            { "then",4, Token::TK_THEN },
            { "if",2, Token::TK_IF },
            { "elseif",6, Token::TK_ELSEIF },
            { "else",4, Token::TK_ELSE },
            { "not",3, Token::TK_NOT },
            { "and",3, Token::TK_AND },
            { "or",2, Token::TK_OR },
            { "function",8, Token::TK_FUNCTION },
            { "break",5, Token::TK_BREAK },
            { "continue",8, Token::TK_CONTINUE },
            { "repeat",6, Token::TK_REPEAT },
            { "until",5, Token::TK_UNTIL },
            { "do",2, Token::TK_DO },
            { "while",5, Token::TK_WHILE },
            { "for",3, Token::TK_FOR },
            { "in",2, Token::TK_IN },
            { "goto",4, Token::TK_GOTO } ,
            { "return",6, Token::TK_RETURN },
    };

    u32 curTokenLength = tokenEnd - tokenBeg;
    for ( int iReverseToken = 0; iReverseToken < sizeof(reverseTokenInfo) / sizeof(reverseTokenInfo[0]); ++iReverseToken )
    {
        if ( reverseTokenInfo[iReverseToken].length == curTokenLength )
        {
            if( !strncmp( tokenBeg, reverseTokenInfo[iReverseToken].token, curTokenLength ) )
            {
                outTokenType = reverseTokenInfo[iReverseToken].tokenType;
                return true;
            }
        }
    }

    return false;
}


bool LexerUtil::IsReverseToken(LXX::s32 tokenType)
{
    return tokenType >=  Token::TK_LOCAL && tokenType < Token::LAST_REVERSED;
}


bool LexerUtil::IsBinaryOp( s32 tokenType )
{
    return
            tokenType == '^' ||
            tokenType == '+' ||
            tokenType == '-' ||
            tokenType == '*' ||
            tokenType == '/' ||
            tokenType == Token::TK_IDIV ||
            tokenType == Token::TK_MOD ||
            tokenType == '&' ||
            tokenType == '~' ||
            tokenType == '|' ||
            tokenType == Token::TK_SHR ||
            tokenType == Token::TK_SHL ||
            tokenType == Token::TK_CONCAT ||
            tokenType == '<' ||
            tokenType == Token::TK_LESSEQUAL ||
            tokenType == '>' ||
            tokenType == Token::TK_GREATEREQUAL ||
            tokenType == Token::TK_EQUAL ||
            tokenType == Token::TK_NOTEQUAL ||
            tokenType == Token::TK_AND ||
            tokenType == Token::TK_OR ;
}


bool LexerUtil::IsUnaryOp(s32 tokenType)
{
    return tokenType == '-'
    ||  tokenType == Token::TK_NOT
    ||  tokenType == '#'
    ||  tokenType == '~';
}


bool LexerUtil::IsTableFieldSeparator( s32 tokenType )
{
    return tokenType == ','
        ||  tokenType == ';';
}


bool LexerUtil::IsBinaryOperator( char ch )
{
    return
        ch == '+' ||
        ch == '-' ||
        ch == '*' ||
        ch == '/' ||
        ch == '%' ||
        ch == '&' ||
        ch == '~' ||
        ch == '|' ||
        ch == '<' ||
        ch == '>' ||
        ch == '^'
        ;
}


bool LexerUtil::IsEndOfToken( char ch )
{
    return ch == ' '
           || ch == '.'
           || ch == ','
           || ch == ':'
           || ch == ';'
           || ch == '('
           || ch == ')'
           || ch == '\t'
           || ch == '\n'
           || ch == '\r'
           || ch == '\v'
           || IsBinaryOperator( ch );
}

void LexerUtil::GetLine(char *destBuff , u32 buffSize , const char* lineStartPos , const char* sourceEndPos )
{
    if( lineStartPos == nullptr ) return;

    const char *_curLinePos = lineStartPos;
    while( _curLinePos[0] != '\n' && _curLinePos < sourceEndPos ) ++ _curLinePos;
    u32 curLineLength = _curLinePos - lineStartPos;
    curLineLength = Min( curLineLength , buffSize - 1 );

    std::strncpy( destBuff , lineStartPos , curLineLength );
}


void LexerUtil::StringToTokenList(const char *str, Array<LXX::String *> &tokens)
{
    const char * currentChar = str;
    const char * endChar = currentChar + std::strlen( str );
    while( currentChar < endChar )
    {
        // skip white space
        while( LexerUtil::CharIsSpace( currentChar[0] ) ) ++currentChar;

        const char* tokenStart = currentChar;
        while( !LexerUtil::CharIsSpace( currentChar[0] ) ) ++currentChar;

        u32 tokenLen = currentChar - tokenStart;
        if( tokenLen > 0 )
        {
            String *strToken = NEW_STRING_WITH_SIZE( tokenLen );
            char *tokenBuff = strToken->GetData();
            std::strncpy(   tokenBuff , tokenStart , tokenLen );
            tokenBuff[tokenLen] = '\0';
            tokens.Add( strToken ) ;
        }
    }
}

} // LXX