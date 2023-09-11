//
// Created by Harry on 8/10/2023.
//
#include "Token.h"
#include <cstring>
#include <cassert>
#include <core/mem/MemoryAllocator.h>
#include "LexerUtil.h"

namespace LXX
{


Token::Token( int tokenType )
{
    this->tokenType = tokenType;
}


IntegerNumberToken::IntegerNumberToken( s32 value )
    : Token( Token::TK_INT )
{
    this->value = value;
}


RealNumberToken::RealNumberToken( f64 value )
    : Token( Token::TK_REAL )
{
    this->value = value;
}


IdentifierToken::IdentifierToken( const char* identifier_beg , const char* identifier_end )
    : Token( Token::TK_IDENTIFY )
{
    u32 len =  identifier_end - identifier_beg;
    assert( len < MAX_NAME_LEN );

    std::strncpy( this->name, identifier_beg, len );
    this->name[len] = '\0';
}


StringToken::StringToken(const char *str_beg, const char *str_end)
        : Token( Token::TK_STRING )
{
    strLen = str_end - str_beg;
    data = (char*) MemoryAllocator::GetInstance().Allocate( strLen + 1);
    std::strncpy( data, str_beg, strLen );
    data[strLen] = '\0';
}


StringToken::StringToken( const StringToken& other )
            : Token( other.tokenType )
{
    strLen = other.strLen;
    data = (char*) MemoryAllocator::GetInstance().Allocate( strLen + 1);
    std::strncpy( data, other.data, strLen );
    data[strLen] = '\0';
}


StringToken::~StringToken()
{
    Dispose();
}


void StringToken::Dispose()
{
    MemoryAllocator::GetInstance().Free( data );
    data = nullptr;
}


ReverseToken::ReverseToken( s32 inTokenType )
    : Token( inTokenType )
{

}


CompatToken::CompatToken()
{

}


CompatToken::~CompatToken()
{
    if( tokenType == Token::TK_STRING )
        stringToken.Dispose();
}


void CompatToken::SetAsNormalToken( s32 tokenType )
{
    this->tokenType = tokenType;
    new(&token) Token( tokenType );
}


void CompatToken::SetAsRealToken( f64 value )
{
    this->tokenType = Token::TK_REAL;
    new(&rToken) RealNumberToken( value );
}


void CompatToken::SetAsIntegerToken( s32 value )
{
    this->tokenType = Token::TK_INT;
    new(&iToken) IntegerNumberToken( value );
}


void CompatToken::SetAsIdentifierToken(const char *identifier_beg, const char *identifier_end) {
    this->tokenType = Token::TK_IDENTIFY;
    new(&identifyToken) IdentifierToken( identifier_beg, identifier_end );
}


void CompatToken::SetAsStringToken( const char *str_beg, const char *str_end )
{
    this->tokenType = Token::TK_STRING;
    new(&stringToken) StringToken( str_beg, str_end );
}


void CompatToken::SetAsReverseToken( s32 tokenType )
{
    this->tokenType = tokenType;
    new(&reverseToken) ReverseToken( tokenType );
}


f64 CompatToken::AsReal() const
{
    assert( tokenType == Token::TK_REAL );
    return rToken.value;
}


s32 CompatToken::AsInteger() const
{
    assert( tokenType == Token::TK_INT );
    return iToken.value;
}


const char* CompatToken::AsString() const
{
    assert( tokenType == Token::TK_STRING );
    return stringToken.GetString();
}


const char* CompatToken::AsIdentifier() const
{
    assert( tokenType == Token::TK_IDENTIFY );
    return identifyToken.GetName();
}


CompatToken& CompatToken::operator=( const CompatToken& other )
{
    tokenType = other.tokenType;
    if(  tokenType == Token::TK_REAL )
        rToken = other.rToken;
    else if(  tokenType == Token::TK_INT )
        iToken = other.iToken;
    else if(  tokenType == Token::TK_IDENTIFY )
        identifyToken = other.identifyToken;
    else if(  tokenType == Token::TK_STRING )
        stringToken = other.stringToken;
    else if( LexerUtil::IsReverseToken( tokenType ) )
        reverseToken = other.reverseToken;
    else
        token = other.token;

    return *this;
}


}
