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
                "local script = loadScript('main')\n"
                "script()\n"
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