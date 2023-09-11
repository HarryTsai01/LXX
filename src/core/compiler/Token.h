//
// Created by Harry on 8/10/2023.
//

#ifndef XLUA_TOKEN_H
#define XLUA_TOKEN_H
#include <core/LXX.h>

namespace LXX
{


class Token
{
    friend class CompatToken;
public:
    enum TokenType
    {
        FIRST_REVERSED = 257,
        TK_LOCAL = FIRST_REVERSED,
        TK_NIL,
        TK_TRUE,
        TK_FALSE,
        TK_END,
        TK_THEN,
        TK_IF,
        TK_ELSEIF,
        TK_ELSE,
        TK_NOT,
        TK_AND,
        TK_OR,
        TK_FUNCTION,
        TK_BREAK,
        TK_CONTINUE,
        TK_REPEAT,
        TK_UNTIL,
        TK_DO,
        TK_WHILE,
        TK_FOR,
        TK_IN,
        TK_GOTO,
        TK_RETURN,
        LAST_REVERSED ,

        /* other token */
        TK_STRING,
        TK_IDENTIFY,
        TK_REAL,
        TK_INT,
        TK_NOTEQUAL,
        TK_EQUAL,
        TK_GREATEREQUAL,
        TK_LESSEQUAL,
        TK_SHL,
        TK_SHR,
        TK_MOD,
        TK_DOT,
        TK_VARARG,
        TK_CONCAT,
        TK_IDIV,
        TK_EOS,
    };
public:
    Token( int tokenType );
    virtual ~Token() = default;

    s32 GetTokenType() const { return tokenType; }
private:
    Token() = default;

protected:
    s32 tokenType;
};


class IntegerNumberToken :  public Token
{
    friend class CompatToken;
public:
    IntegerNumberToken( s32 value );
private:
    IntegerNumberToken() = default;
private:
    s32 value;
};


class RealNumberToken : public Token
{
    friend class CompatToken;
public:
    RealNumberToken( f64 value );

private:
    RealNumberToken() = default;

private:
    f64 value;
};


class IdentifierToken : public Token
{
    friend class CompatToken;
public:
    IdentifierToken( const char* identifier_beg , const char* identifier_end );

    const char* GetName() const { return name; }

private:
    IdentifierToken() = default;

private:
    static constexpr const s32 MAX_NAME_LEN = 256;
    char name[MAX_NAME_LEN];
};


class ReverseToken : public Token
{
public:
    ReverseToken( s32 inTokenType );
};


class StringToken : public Token
{
    friend class CompatToken;
public:
    StringToken( const char* str_beg , const char* str_end );
    StringToken( const StringToken& other );
    virtual ~StringToken();
    void Dispose() ;
    const char* GetString() const { return data; }

private:
    StringToken() = default;

private:
    char* data;
    u32 strLen;
};


class CompatToken : public Token
{
public:
    CompatToken() ;
    virtual ~CompatToken();

    s32 GetTokenType() const { return tokenType; }

    void SetAsNormalToken( s32 tokenType );
    void SetAsRealToken( f64 value );
    void SetAsIntegerToken( s32 value );
    void SetAsIdentifierToken( const char* identifier_beg , const char* identifier_end );
    void SetAsStringToken( const char* str_beg , const char* str_end );
    void SetAsReverseToken( s32 tokenType );

    f64 AsReal() const;
    s32 AsInteger() const;
    const char* AsIdentifier() const;
    const char* AsString() const;

    CompatToken& operator=( const CompatToken& other );
private:
    union
    {
        Token token;
        RealNumberToken rToken;
        IntegerNumberToken iToken;
        IdentifierToken identifyToken;
        StringToken stringToken;
        ReverseToken reverseToken;
    };
};
} // LXX

#endif //XLUA_TOKEN_H
