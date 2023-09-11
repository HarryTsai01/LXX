//
// Created by Harry on 8/27/2023.
//

#ifndef XLUA_TESTCASE04_H
#define XLUA_TESTCASE04_H
#include "TestCaseBase.h"

namespace LXXTest
{

class TestCase04 : public TestCaseBase
{
    TEST_CASE_DECLARE( TestCase04 , TestCaseBase )
public:

    bool Run() override;

};

} // LXXTest

#endif //XLUA_TESTCASE04_H