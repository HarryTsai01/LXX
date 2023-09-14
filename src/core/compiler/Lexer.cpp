//
// Created by Harry on 8/10/2023.
//

#include "Lexer.h"
#include "LexerUtil.h"
#include <fstream>
#include <sstream>
#include <cmath>
#include <core/exceptions/ExceptionCompileError.h>
#include <cstdio>
#include <cstring>
#include <core/objects/string/StringTable.h>
#include <core/objects/string/StringUtil.h>

namespace LXX
{

Lexer::Lexer()
    : sourceFileName( nullptr )
    , scriptContent( nullptr )
    , currentChar(nullptr )
    , endChar(nullptr )
    , lineNo( 1 )
    , savedCurrentChar( nullptr )
    , savedLineNo( -1 )
{

}


bool Lexer::LoadScriptContent( String *scriptContent , String *sourceFileName )
{
    this->scriptContent = scriptContent;
    this->sourceFileName = sourceFileName;

    currentChar = scriptContent->GetData();
    endChar = currentChar + scriptContent->GetLength();

    return  true;
}


bool Lexer::LookAheadToken( CompatToken& token )
{
    const char* _currentChar = currentChar;
    u32 _lineNo = lineNo;
    return NextTokenImpl(token , _currentChar , endChar , _lineNo , currentLineStart );
}


bool Lexer::LookAheadTokenIf( CompatToken& token , bool (*checkFunc)( s32 tokenType ) )
{
    return  LookAheadToken( token ) && checkFunc( token.GetTokenType() );
}


bool Lexer::LookAheadTokenIf( CompatToken& token , s32 tokenType )
{
    return LookAheadToken( token ) && token.GetTokenType() == tokenType ;
}


bool Lexer::LookAheadTokenIf( CompatToken& token , s32 tokenType1 , s32 tokenType2 )
{
    return LookAheadToken( token ) &&
    (
        token.GetTokenType() == tokenType1
        || token.GetTokenType() == tokenType2
    );
}


bool Lexer::LookAheadTokenIf( CompatToken& token , s32 tokenType1 , s32 tokenType2 ,  s32 tokenType3 )
{
    return LookAheadToken( token ) &&
           (
                  token.GetTokenType() == tokenType1
                  || token.GetTokenType() == tokenType2
                  || token.GetTokenType() == tokenType3
           );
}


bool Lexer::NextToken( CompatToken &token )
{
    return NextTokenImpl(token , currentChar , endChar , lineNo , currentLineStart );
}


bool Lexer::NextTokenIf( CompatToken& token , bool (*checkFunc)( s32 tokenType ) )
{
    if( LookAheadTokenIf( token , checkFunc ) )
        return NextToken( token );
    return false;
}


bool Lexer::NextTokenIf(CompatToken &token, LXX::s32 tokenType )
{
    if( LookAheadToken( token ) && token.GetTokenType() == tokenType )
        return NextToken( token );

    return false;
}

bool Lexer::NextTokenIf(  CompatToken& token , s32 tokenType1 , s32 tokenType2 )
{
    if( LookAheadToken( token ) &&
            (
                    token.GetTokenType() == tokenType1
                    || token.GetTokenType() == tokenType2
            )
        )
        return NextToken( token );

    return false;
}


bool Lexer::NextTokenIf(  CompatToken& token , s32 tokenType1 , s32 tokenType2 , s32 tokenType3 )
{
    if( LookAheadToken( token ) &&
        (
                token.GetTokenType() == tokenType1
                || token.GetTokenType() == tokenType2
                || token.GetTokenType() == tokenType3
        )
    )
        return NextToken( token );

    return false;
}


bool Lexer::NextTokenImpl( CompatToken& token  , const char* & _currentChar , const char* & _endChar , u32 & _lineNo , const char *&_currentLineStart )
{
#define SKIP_WHITESPACE() { \
        while ( _currentChar < _endChar \
            && (            \
                _currentChar[0] == ' ' \
                || _currentChar[0] == '\t' \
                || _currentChar[0] == '\v' \
                ) \
            ) \
        {                   \
            ++ _currentChar; \
        }                   \
    }

    while (_currentChar < _endChar )
    {
        switch ( _currentChar[0] )
        {
        case '\r':
            _currentChar += 1;
            break;
        case '\n':
            _currentChar += 1;
            ++ _lineNo;
            _currentLineStart = _currentChar;
            break;
        case ' ':
        case '\t':
        case '\v':
            SKIP_WHITESPACE();
            break;
        case '-' :
            {
                if (_currentChar + 1 < _endChar )
                {
                    if (_currentChar[1] == '-' )
                    {
                        if (_currentChar + 3 < _endChar
                            && _currentChar[2] == '['
                            && _currentChar[3] == '[' )
                        {
                            _currentChar += 4;
                            // skip multi line comment
                            bool bMultiLineCommentCompleted = false;
                            while (_currentChar < _endChar )
                            {
                                if (_currentChar + 3 < _endChar
                                    && _currentChar[0] == '-'
                                    && _currentChar[1] == '-'
                                    && _currentChar[2] == ']'
                                    && _currentChar[3] == ']'
                                    )
                                {
                                    _currentChar += 4;
                                    bMultiLineCommentCompleted = true;
                                    break;
                                }
                                else
                                {
                                    if(_currentChar[0] == '\n' )
                                    {
                                        ++ _lineNo;
                                        _currentLineStart = _currentChar + 1;
                                    }
                                    ++ _currentChar;
                                }
                            }

                            if ( !bMultiLineCommentCompleted )
                                ThrowErrorImpl( "Multi line comment not completed"  , _lineNo , _currentLineStart );
                        }
                        else
                        {
                            // skip single line comment
                            while (_currentChar < _endChar && _currentChar[0] != '\n' )
                                ++ _currentChar;
                        }
                    }
                    else
                    {
                        token.SetAsNormalToken( _currentChar[0] );
                        ++ _currentChar;
                        return true;
                    }
                }
            }
            break;
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            {
                if ( _currentChar[0] == '0' )
                {
                    ++ _currentChar;
                    bool bIsHex = false;
                    bool bIsBinary = false;
                    bool bIsOctal = false;
                    if(_currentChar[0] == 'x' || _currentChar[0] == 'X' ) { bIsHex  = true;  ++ _currentChar; }
                    else if(_currentChar[0] == 'b' || _currentChar[0] == 'B' ) { bIsBinary  = true;  ++ _currentChar; }
                    else bIsOctal = true;
                    const char* numStart = _currentChar;
                    while (_currentChar < _endChar )
                    {
                        if (_currentChar[0] >= '0' && _currentChar[0] <= '9'
                            || _currentChar[0] >= 'a' && _currentChar[0] <= 'f'
                            || _currentChar[0] >= 'A' && _currentChar[0] <= 'F'
                             )
                        {
                            ++ _currentChar;
                        }
                        else if (LexerUtil::IsEndOfToken(_currentChar[0]) )
                        {
                            break;
                        }
                        else
                        {
                            ThrowErrorImpl( "Invalid number" , _lineNo , _currentLineStart );
                        }
                    }
                    const char * numEnd = _currentChar - 1;

                    if ( numStart == numEnd )
                        ThrowErrorImpl( "Invalid number" , _lineNo , _currentLineStart );

                    u32 num = 0 ;
                    u32 bitIndex = 0 ;
                    const char *curNum = numStart ;
                    while ( curNum <= numEnd )
                    {
                        char curVal = curNum[0] ;
                        if ( bIsHex )
                        {
                            if( curVal >= 'a' && curVal <= 'f' )
                                curVal = 10 + curVal - 'a' ;
                            else if( curVal >= 'A' && curVal <= 'F' )
                                curVal = 10 + curVal - 'A' ;
                            else
                                curVal -= '0' ;

                            num <<=  4;
                            num += curVal;
                        }
                        else if ( bIsOctal )
                        {
                            if ( curVal >= '0' && curVal <= '7' )
                                curVal -= '0' ;
                            else
                                ThrowErrorImpl( "Invalid number" , _lineNo , _currentLineStart );
                            num <<=  3;
                            num += curVal;
                        }
                        else if ( bIsBinary )
                        {
                            if  ( curVal >= '0' && curVal <= '1' )
                                curVal -= '0' ;
                            else
                                ThrowErrorImpl( "Invalid number" , _lineNo , _currentLineStart );
                            num <<= 1;
                            num += curVal;
                        }

                        ++ curNum;
                    }

                    token.SetAsIntegerToken( num );
                    return true;
                }
                else
                {
                    const char* numStart = _currentChar;
                    bool bHasDot = false;
                    while ( _currentChar < _endChar )
                    {
                        if ( _currentChar[0] >= '0' && _currentChar[0] <= '9' || _currentChar[0] == '.' )
                        {
                            if (_currentChar[0] == '.' )
                            {
                                if ( bHasDot )
                                    ThrowErrorImpl( "Invalid number, Multiple dots" , _lineNo , _currentLineStart );

                                bHasDot = true;
                            }
                            ++_currentChar;
                        }
                        else if (LexerUtil::IsEndOfToken(_currentChar[0]) )
                        {
                            break;
                        }
                        else
                        {
                            ThrowErrorImpl( "Invalid number" , _lineNo , _currentLineStart );
                        }
                    }
                    const char *numEnd = _currentChar - 1;

                    u32 num = 0 ;
                    f32  fNum = 0.0f;
                    const char *curNum = numStart ;
                    bool bProcessDot = false;
                    u32 floatIndex = 1;
                    while ( curNum <= numEnd )
                    {
                        char curVal = curNum[0] ;
                        if ( curVal == '.' )
                        {
                            bProcessDot = true;
                        }
                        else
                        {
                            if ( bProcessDot )
                                fNum +=  (curVal - '0') * pow( 0.1f , floatIndex++ );
                            else
                                num = num * 10 + curVal - '0' ;
                        }
                        ++ curNum;

                    }

                    if ( bHasDot )
                        token.SetAsRealToken( num + fNum );
                    else
                        token.SetAsIntegerToken( num );
                    return true;
                }
            }
            break;
        case '\"': case '\'':
            {
                bool bSingleQuote = _currentChar[0] == '\'';
                // skip quote
                ++ _currentChar;
                const char* strStart = _currentChar ;
                while (_currentChar < _endChar )
                {
                    if (_currentChar[0] == '\r' || _currentChar[0] == '\n' )
                        ThrowErrorImpl( "Invalid string , Expected '\'' or '\"' " ,  _lineNo , _currentLineStart );
                    else if (_currentChar[0] == '\'' && bSingleQuote
                             || _currentChar[0] == '\"' && !bSingleQuote )
                    {
                        token.SetAsStringToken(strStart , _currentChar );
                        _currentChar += 1;
                        return true;
                    }
                    ++ _currentChar;
                }
                ThrowErrorImpl( "Invalid string , Expected '\'' or '\"' " ,  _lineNo , _currentLineStart );
            }
            break;
        case '[':
            {
                /*
                 * process long string
                 * [[ comment content ]]
                 * [=[ comment content ]=]
                 * */
                bool  bLongString1 = _currentChar + 1 < _endChar && _currentChar[1] == '[';
                bool bLongString2 = _currentChar + 2 < _endChar && _currentChar[1] == '=' && _currentChar[2] == '[';
                if ( bLongString1 || bLongString2 )
                {
                    if( bLongString1 ) _currentChar += 2;
                    else if( bLongString2 ) _currentChar += 3;

                    const char* str_beg = _currentChar;
                    bool bCompleteString = false;
                    while (_currentChar < _endChar )
                    {
                        if(bLongString1 && _currentChar + 1 < _endChar && _currentChar[0] == ']' && _currentChar[1] == ']'
                        || bLongString2 && _currentChar + 2 < _endChar && _currentChar[0] == ']' && _currentChar[1] == '=' && _currentChar[2] == ']' )
                        {
                            bCompleteString = true;
                            break;
                        }

                        if (  _currentChar[0] == '\n' )
                        {
                            ++ _lineNo;
                            _currentLineStart = _currentChar + 1 ;
                        }

                        ++ _currentChar;
                    }

                    if( !bCompleteString )
                        ThrowErrorImpl( "Invalid long string" , _lineNo , _currentLineStart );

                    token.SetAsStringToken(str_beg , _currentChar );

                    if( bLongString1 ) _currentChar += 2;
                    else if( bLongString2 ) _currentChar += 3;

                    return true;
                }
                else
                {
                    token.SetAsNormalToken(_currentChar[0] );
                    _currentChar += 1;
                    return true;
                }
            }
            break;
        case '.':
            {
                if(_currentChar + 1 < _endChar && _currentChar[1] == '.' )
                {
                    if( _currentChar + 2 < _endChar && _currentChar[2] == '.' )
                    {
                        _currentChar += 3;
                        token.SetAsNormalToken( Token::TK_DOT );
                    }
                    else
                    {
                        _currentChar += 2;
                        token.SetAsNormalToken( Token::TK_CONCAT );
                    }
                    return true;
                }
                else if(_currentChar + 1 < _endChar && _currentChar[1] >= '0' && _currentChar[1] <= '9' )
                {
                    // skip '.'
                    ++ _currentChar;

                    f64 fNum = 0.0;
                    u32  floatIndex = 1;
                    while (_currentChar < _endChar )
                    {
                        if (_currentChar[0] >= '0' && _currentChar[0] <= '9' )
                        {
                            fNum += (_currentChar[0] - '0') * pow(0.1 , floatIndex++ );
                            ++ _currentChar;
                        }
                        else if (LexerUtil::IsEndOfToken(_currentChar[0]) )
                        {
                            break;
                        }
                        else
                        {
                            ThrowErrorImpl( "Invalid number" , _lineNo , _currentLineStart );
                        }
                    }

                    token.SetAsRealToken( fNum );
                    return true;
                }
                else
                {
                    _currentChar += 1;
                    token.SetAsNormalToken( '.' );
                    return true;
                }
            }
            break;
        case '~':
            {
                if (_currentChar + 1 < _endChar && _currentChar[1] == '=' )
                {
                    _currentChar += 2;
                    token.SetAsNormalToken( Token::TK_NOTEQUAL );
                }
                else
                {
                    token.SetAsNormalToken(_currentChar[0] );
                    _currentChar += 1;
                }
                return true;
            }
            break;
        case '=':
            {
                if (_currentChar + 1 < _endChar && _currentChar[1] == '=' )
                {
                    _currentChar += 2;
                    token.SetAsNormalToken( Token::TK_EQUAL );
                }
                else
                {
                    token.SetAsNormalToken(_currentChar[0] );
                    _currentChar += 1;
                }
                return true;
            }
            break;
        case '>':
            {
                if (_currentChar + 1 < _endChar && _currentChar[1] == '=' )
                {
                    _currentChar += 2;
                    token.SetAsNormalToken( Token::TK_GREATEREQUAL );
                }
                else if(_currentChar + 1 < _endChar && _currentChar[1] == '>' )
                {
                    _currentChar += 2;
                    token.SetAsNormalToken( Token::TK_SHR );
                }
                else
                {
                    token.SetAsNormalToken(_currentChar[0] );
                    _currentChar += 1;
                }
                return true;
            }
            break;
        case '<':
            {
                if (_currentChar + 1 < _endChar && _currentChar[1] == '=' )
                {
                    _currentChar += 2;
                    token.SetAsNormalToken( Token::TK_LESSEQUAL );
                }
                else if(_currentChar + 1 < _endChar && _currentChar[1] == '<' )
                {
                    _currentChar += 2;
                    token.SetAsNormalToken( Token::TK_SHL );
                }
                else
                {
                    token.SetAsNormalToken(_currentChar[0] );
                    _currentChar += 1;
                }
                return true;
            }
            break;
        case '%':
            {
                token.SetAsNormalToken( Token::TK_MOD );
                _currentChar += 1;
                return true;
            }
            break;
        case '/':
            {
                if (_currentChar + 1 < _endChar && _currentChar[1] == '/' )
                {
                    _currentChar += 2;
                    token.SetAsNormalToken( Token::TK_IDIV );
                }
                else
                {
                    token.SetAsNormalToken(_currentChar[0] );
                    _currentChar += 1;
                }
                return true;
            }
            break;
        case ':':
        case '+':
        case '*':
        case '(':
        case ')':
        //case '[':
        case ']':
        case '{':
        case '}':
        case ',':
        case ';':
        case '#':
        case '^':
        case '|':
        case '&':
            {
                token.SetAsNormalToken(_currentChar[0] );
                _currentChar += 1;
                return true;
            }
            break;
        default:
            {
                // identifier

                // check the first char is a letter or underscore
                if (_currentChar[0] >= 'a' && _currentChar[0] <= 'z'
                    || _currentChar[0] >= 'A' && _currentChar[0] <= 'Z'
                    || _currentChar[0] == '_' )
                {
                    const char *identifier_beg = _currentChar;
                    while (_currentChar < _endChar )
                    {
                        if (_currentChar[0] >= 'a' && _currentChar[0] <= 'z'
                            || _currentChar[0] >= 'A' && _currentChar[0] <= 'Z'
                            || _currentChar[0] >= '0' && _currentChar[0] <= '9'
                            || _currentChar[0] == '_' )
                        {
                            ++ _currentChar;
                        }
                        // check if it is the end of identifier
                        else if ( LexerUtil::IsEndOfToken( _currentChar[0] ) )
                        {
                            break;
                        }
                        else
                        {
                            ThrowErrorImpl( "Invalid identifier" , _lineNo , _currentLineStart );
                        }
                    }
                    // check if it is a reverse token
                    s32 tokenType;
                    if ( LexerUtil::IsReverseToken(identifier_beg , _currentChar  , tokenType ) )
                    {
                        token.SetAsReverseToken( tokenType );
                    }
                    else
                    {
                        token.SetAsIdentifierToken( identifier_beg , _currentChar );
                    }
                    return true;
                }
                else
                {
                    ThrowErrorImpl( "Invalid identifier" , _lineNo , _currentLineStart );
                }

            }
            break;
        }
    }

    return false;
}


void Lexer::CollectReferences( GCObjectCollector& collector )
{
}


void Lexer::ThrowErrorImpl( String *message , u32 _lineNo , const char *_currentLineStart )
{
    char currentLine[256] = {0};
    LexerUtil::GetLine( currentLine , 256 , _currentLineStart , endChar );

    throw ExceptionCompileError( message , sourceFileName , NEW_STRING( currentLine ) , _lineNo );
}


void Lexer::SaveCurrentPosition()
{
    savedLineNo = lineNo;
    savedCurrentChar = currentChar;
}


void Lexer::RestoreCurrentPosition()
{
    lineNo = savedLineNo;
    currentChar = savedCurrentChar;
}

} // LXX