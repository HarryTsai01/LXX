//
// Created by Harry on 8/24/2023.
//

#ifndef XLUA_TESTCASE02_H
#define XLUA_TESTCASE02_H
#include "TestCaseBase.h"

namespace LXXTest {

class TestCase02 : public TestCaseBase
{
    TEST_CASE_DECLARE( TestCase02 , TestCaseBase )
public:
    bool Run() override;
};

} // LXXTest

#endif //XLUA_TESTCASE02_H
