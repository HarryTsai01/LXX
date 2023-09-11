//
// Created by Harry on 9/2/2023.
//

#ifndef XLUA_INSTRUCTIONSETTESTCASE_H
#define XLUA_INSTRUCTIONSETTESTCASE_H
#include "TestCaseBase.h"

namespace LXXTest {

class InstructionSetTestCase : public TestCaseBase
{
    TEST_CASE_DECLARE(InstructionSetTestCase , TestCaseBase )
public:
    bool Run() override;
};

} // LXXTest

#endif //XLUA_INSTRUCTIONSETTESTCASE_H
