//
// Created by Harry on 9/13/2023.
//
#include "api.h"
#include <core/GlobalState.h>
#include <core/objects/Table.h>

namespace LXX
{

u32 LXX_Assert( State *state )
{
    Stack& stack = state->GetStack();
    if( !stack.IsBoolean( -1 ) )
    {
        state->ThrowError( "assert function requires boolean argument" );
        return 0;
    }

    if( !stack.GetBoolean( -1 ) )
    {
        state->ThrowError( "assert failed" );
        return 0;
    }

    return 0;
}


}
