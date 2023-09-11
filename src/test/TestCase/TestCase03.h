//
// Created by Harry on 8/26/2023.
//

#ifndef XLUA_TESTCASE03_H
#define XLUA_TESTCASE03_H
#include "TestCaseBase.h"

namespace LXXTest
{


class TestCase03 :  public TestCaseBase
{
TEST_CASE_DECLARE( TestCase03 , TestCaseBase )
public:
    bool Run() override;
}; // class TestCase03


} // namespace LXXTest



#endif //XLUA_TESTCASE03_H
