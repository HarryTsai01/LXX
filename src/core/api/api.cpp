//
// Created by Harry on 9/13/2023.
//
#include "api.h"
#include <core/GlobalState.h>
#include <core/objects/Table.h>
#include <core/log/log.h>

namespace LXX
{

using namespace LOG;

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


u32 LXX_Print( State *state )
{
    Stack& stack = state->GetStack();
    u32 argumentNum = state->GetCurrentCallInfo()->GetActualArgumentNum();
    if( argumentNum == 0 )
        state->ThrowError("`print` function expect one argument at list. ");

    u32 curArgumentIdx = 0 ;
    while( curArgumentIdx < argumentNum )
    {
        auto curArgument = stack.GetValue( curArgumentIdx + 1 );

        Log( LogCategory::LXX , "%s", curArgument->ToString()->GetData() );

        ++curArgumentIdx;
    }

    return 0;
}


}
