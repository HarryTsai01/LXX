//
// Created by Harry on 8/10/2023.
//

#include "Compiler.h"
#include <iostream>
#include <core/Value.h>
#include <core/compiler/AST/FunctionStatement.h>
#include <core/vm/ByteCodeChunk.h>
#include <core/vm/Instructions/InstructionEncoder.h>
#include <core/exceptions/ExceptionCompileError.h>
#include <core/log/log.h>

namespace LXX
{
Compiler::Compiler()
    : scriptFileName( nullptr )
    , lexer( nullptr )
    , parser( nullptr )
{

}


Compiler::~Compiler()
{

}

LuaClosure* Compiler::CompileString( const char* scriptContent )
{
    return CompileStringImpl(
            NEW_STRING(scriptContent)
    );
}


LuaClosure* Compiler::CompileFile( String* scriptFileName )
{
    this->scriptFileName = scriptFileName;
    String* scriptContent = LoadScriptContent( scriptFileName );
    return CompileStringImpl( scriptContent );
}


StatementBase* Compiler::CompileAST( String* scriptContent )
{
    lexer = new Lexer();
    lexer->LoadScriptContent( scriptContent , scriptFileName );

    parser = new Parser();
    parser->SetLexer( lexer );

    FunctionStatement *functionStatement = parser->CompileLuaClosure();
    if( functionStatement == nullptr )
    {
        LOG::LogError( LOG::LogCategory::LXX , "%s" , "Compile failed" );
        return nullptr;
    }
    return functionStatement;
}


LuaClosure* Compiler::CompileStringImpl( String* scriptContent )
{
    StatementBase* statement = CompileAST( scriptContent );
    if( statement != nullptr )
    {
        return CompileStatement( statement );
    }
    return nullptr;
}


void Compiler::CollectReferences( GCObjectCollector& collector )
{
    collector.AddObject( lexer );
    collector.AddObject( parser );
    collector.AddObject( scriptFileName );
}


String* Compiler::LoadScriptContent( String* scriptFileName )
{
    this->scriptFileName = scriptFileName;

    FILE *fp = fopen( scriptFileName->GetData() , "r" );
    if ( fp == nullptr )
    {
        ThrowError( "File not found" );
    }

    // read all script content from file
    fseek( fp , 0 , SEEK_END );
    long fileSize = ftell( fp );
    fseek( fp , 0 , SEEK_SET );
    String* scriptContent = NEW_STRING_WITH_SIZE( fileSize );
    fread( scriptContent->GetData() , 1 , fileSize , fp );

    return scriptContent;
}


void Compiler::ThrowError( const char* message )
{
    //throw ExceptionCompileError( message ,  scriptFileName , -1 );
    lexer->ThrowError( message );
}

} // LXX