//
// Created by Harry on 9/2/2023.
//

#include "InstructionSetTestCase.h"
#include <core/vm/Instructions/InstructionEncoder.h>

namespace LXXTest
{

using namespace Instruction;

InstructionSetTestCase::InstructionSetTestCase()
{
    m_scriptContents =
            {
                "g = 100 \n"
                "local a = '12' \n"
                "a = 1 \n"
                "local b = 2 \n"
                "local c = a..b \n"
                "assert( c == '12') \n"
                "assert(a == 1) \n"
                "a = 1 + 2 \n"
                "assert(a == 3) \n"
                "a = a + 1 \n"
                "assert(a == 4) \n"
                "a = a - 1 \n"
                "assert(a == 3) \n"
                "a = a * 2 \n"
                "assert(a == 6) \n"
                "a = a / 2 \n"
                "assert(a == 3) \n"
                "a = 9 // 2 \n"
                "assert(a == 4) \n"
                "a = 9 % 2 \n"
                "assert(a == 1) \n"
            };
}


bool InstructionSetTestCase::Run()
{

    VirtualMachine vm;
    if( vm.Startup() )
    {
        for( auto& script : m_scriptContents )
        {
            try
            {
                if( !vm.Execute( script ) )
                    return false;
            }
            catch( ExceptionBase &e )
            {
                std::cout << e.ToString() << std::endl;
                vm.Shutdown();
                return false;
            }
        }

        vm.Shutdown();
        return true;
    }

    return false;
}


} // LXXTest