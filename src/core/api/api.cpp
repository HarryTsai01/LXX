//
// Created by Harry on 9/13/2023.
//
#include "api.h"
#include <core/GlobalState.h>
#include <core/objects/Table.h>
#include <core/log/log.h>
#include <core/vm/VirtualMachine.h>

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
        state->ThrowError("`print` function expect one argument at least. ");

    u32 curArgumentIdx = 0 ;
    while( curArgumentIdx < argumentNum )
    {
        auto curArgument = stack.GetValue( - curArgumentIdx - 1  );

        LogDisplay( LogCategory::LXX , "%s", curArgument->ToString()->GetData() );

        ++curArgumentIdx;
    }

    return 0;
}


u32 LXX_LoadScript( State *state )
{
    u32 argumentNum = state->GetCurrentCallInfo()->GetActualArgumentNum();
    if( argumentNum != 1 )
        state->ThrowError("`loadScript` function expect one argument . ");

    Stack &stack = state->GetStack();
    String* scriptPath = stack.As< String*>( -1 );

    LuaClosure* luaClosure = state->GetVM()->CompileFile( String::Format("asserts/scripts/%s.lxx" , scriptPath->GetData() ) );

    stack.Push( luaClosure );

    return 1;
}


}
