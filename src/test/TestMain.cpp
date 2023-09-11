//
// Created by Harry on 8/4/2023.
//
#include <iostream>
#include <memory>
#include <vector>
#include "TestCase/TestCase01.h"
#include "TestCase/TestCase02.h"
#include "TestCase/TestCase03.h"
#include "TestCase/TestCase04.h"
#include "TestCase/CompilerTestCase.h"
#include "TestCase/InstructionSetTestCase.h"

using namespace std;
using namespace LXXTest;

vector< shared_ptr< TestCaseBase > > TestCases =
        {
                make_shared< TestCase01 >( ),
                make_shared< TestCase02 >( ),
                make_shared< TestCase03 >( ),
                make_shared< TestCase04 >( ),
                make_shared< CompilerTestCase >( ),
                make_shared< InstructionSetTestCase >( ),
        };


int main( )
{
    for ( auto testCase : TestCases )
    {
        if( ! testCase->Run() )
        {
            cerr << "Test Case: " << testCase->GetTestName() << " Failed!" << endl;
            cerr << "Error Message: " << testCase->GetErrorDetail() << endl;
            return -1;
        }
    }

    cout << "All Test Passed!" << endl;

    return 0;
}
