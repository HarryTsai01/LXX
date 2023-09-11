//
// Created by Harry on 8/10/2023.
//

#ifndef XLUA_LEXER_H
#define XLUA_LEXER_H
#include <string>
#include "Token.h"
#include <core/objects/GCObject.h>
#include <core/objects/GCObjectCollector.h>
#include <core/objects/string/String.h>

namespace LXX
{

class Lexer : public GCObject
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL

    friend class Parser;
public:
    Lexer();
    virtual void CollectReferences( GCObjectCollector& collector ) ;
    bool LoadScriptContent( String *scriptContent , String *sourceFileName );
    bool NextToken( CompatToken& token );
    bool NextTokenIf( CompatToken& token , bool (*checkFunc)( s32 tokenType ) );
    bool NextTokenIf(  CompatToken& token , s32 tokenType );
    bool NextTokenIf(  CompatToken& token , s32 tokenType1 , s32 tokenType2 );
    bool NextTokenIf(  CompatToken& token , s32 tokenType1 , s32 tokenType2 , s32 tokenType3 );
    bool LookAheadToken( CompatToken& token );
    bool LookAheadTokenIf( CompatToken& token , bool (*checkFunc)( s32 tokenType ) );
    bool LookAheadTokenIf( CompatToken& token , s32 tokenType );
    bool LookAheadTokenIf( CompatToken& token , s32 tokenType1 , s32 tokenType2 );
    bool LookAheadTokenIf( CompatToken& token , s32 tokenType1 , s32 tokenType2 ,  s32 tokenType3 );

    void SaveCurrentPosition();
    void RestoreCurrentPosition();

private:
    bool NextTokenImpl( CompatToken& token  , const char* & _currentChar , const char* & _endChar , u32 & _lineNo);
    void ThrowError( const char *message );
    void ThrowErrorImpl( const char *message , u32 _lineNo );
private:
    String* sourceFileName;
    String* scriptContent;
    const char* CurrentChar;
    const char* EndChar;
    u32 lineNo;

    const char* savedCurrentChar;
    u32 savedLineNo;
};

} // LXX

#endif //XLUA_LEXER_H
