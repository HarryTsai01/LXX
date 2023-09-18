//
// Created by Harry on 9/18/2023.
//

#include "CommandSystem.h"
#include <core/compiler/LexerUtil.h>

namespace LXX
{


bool CommandSystem::Process( const char *commands )
{
    Array< String* > tokens;

    LexerUtil::StringToTokenList( commands , tokens );

    if( tokens.Size() == 0 )
        return false;

    auto It = _commandEvents.Find(tokens[0] );
    if( It != _commandEvents.End() )
    {
        It->Second( tokens );
        return true;
    }

    return false;
}


} // LXX