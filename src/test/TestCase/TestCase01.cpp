//
// Created by Harry on 8/4/2023.
//

#include "TestCase01.h"
#include <core/vm/VirtualMachine.h>
#include <core/exceptions/ExceptionInvalidParameter.h>

namespace LXXTest
{

static u32 Add( State* state )
{
    Stack&  stack = state->GetStack();

    u32 a = stack.GetInteger( -1 , true );
    u32 b = stack.GetInteger( -2 , true );
    stack.PushInteger( a + b );

    return 1;
}

bool TestCase01::Run()
{
    VirtualMachine vm;
    if( vm.Startup() )
    {
        s32 result = 0;
        if( !vm.InvokeWithResult( &Add , result , 2 , 1 ) )
        {
            vm.Shutdown();
            return false;
        }

        DO_TEST( result == 3 );

        //state->GetStack().Pop( 1 );

        vm.Shutdown();
        return true;
    }

    return false;
}


}


