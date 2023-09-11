//
// Created by Harry on 9/11/2023.
//
#include "VirtualMachine.h"

namespace LXX
{

template<>
u32 Hash( VMSystemFunctionKey key )
{
    return key.Hash();
}


void VirtualMachine::OnSystemFunction( u32 systemFunction , u32 argument1 , u32 argument2 )
{
    auto it = _systemFunctionBindings.Find( VMSystemFunctionKey( systemFunction , argument1 , argument2 ) );
    if( it == _systemFunctionBindings.End() )
        ThrowError( "System function not found" );
    it->Second->Invoke();
}


void VirtualMachine::RegisterSystemFunction()
{
    BEGIN_REGISTER_SYSTEM_FUNCTION()
        REGISTER_SYSTEM_FUNCTION_ITEM( RaiseException , SystemException , ExceptionForZeroReturnValueCount )
        REGISTER_SYSTEM_FUNCTION_ITEM( RaiseException , SystemException , ExceptionForStatementLimitExpressionIsNotNumber )
        REGISTER_SYSTEM_FUNCTION_ITEM( RaiseException , SystemException , ExceptionForStatementIncreaseStepIsNotNumberAndIsNotNil )
        REGISTER_SYSTEM_FUNCTION_ITEM( RaiseException , SystemException , ExceptionForStatementIncreaseStepIsEqualZero )
    END_REGISTER_SYSTEM_FUNCTION()
}

IMPLEMENTATION_SYSTEM_FUNCTION( RaiseException , ExceptionForZeroReturnValueCount )
{
    ThrowError( "[VirtualMachine] [ SystemException ] [ForStatement] invalid expression list , return value count is zero " );
}


IMPLEMENTATION_SYSTEM_FUNCTION( RaiseException , ExceptionForStatementLimitExpressionIsNotNumber )
{
    ThrowError( "[VirtualMachine] [ SystemException ] [ForStatement] invalid expression list , limit expression is not number " );
}


IMPLEMENTATION_SYSTEM_FUNCTION( RaiseException , ExceptionForStatementIncreaseStepIsNotNumberAndIsNotNil )
{
    ThrowError( "[VirtualMachine] [ SystemException ] [ForStatement] invalid expression list , increase step is not number and not nil " );
}


IMPLEMENTATION_SYSTEM_FUNCTION( RaiseException , ExceptionForStatementIncreaseStepIsEqualZero )
{
    ThrowError( "[VirtualMachine] [ SystemException ] [ForStatement] invalid expression list , increase step is equal zero " );
}

}