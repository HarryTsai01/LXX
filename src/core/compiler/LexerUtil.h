//
// Created by Harry on 8/16/2023.
//

#ifndef XLUA_LEXERUTIL_H
#define XLUA_LEXERUTIL_H
#include <core/LXX.h>

namespace LXX {


class LexerUtil
{
public:
    static bool IsReverseToken( const char* tokenBeg , const char* tokenEnd , s32 &outTokenType);
    static bool IsReverseToken( s32 tokenType);
    static bool IsBinaryOp( s32 tokenType );
    static bool IsUnaryOp( s32 tokenType );
    static bool IsTableFieldSeparator( s32 tokenType );

    static bool IsBinaryOperator( char ch );
    static bool IsEndOfToken(char ch );

    static void GetLine(char *destBuff , u32 buffSize , const char* lineStartPos , const char* sourceEndPos );
};


} // LXX

#endif //XLUA_LEXERUTIL_H
