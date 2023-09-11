//
// Created by Harry on 8/4/2023.
//

#ifndef XLUA_TESTCASE01_H
#define XLUA_TESTCASE01_H
#include "TestCaseBase.h"

namespace LXXTest
{

class TestCase01 : public TestCaseBase
{
    TEST_CASE_DECLARE( TestCase01 , TestCaseBase )
public:
    bool Run() override;
};  // class TestCase01


} // namespace LXXTest



#endif //XLUA_TESTCASE01_H
