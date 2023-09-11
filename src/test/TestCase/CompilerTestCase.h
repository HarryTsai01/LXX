//
// Created by Harry on 8/27/2023.
//

#ifndef XLUA_COMPILERTESTCASE_H
#define XLUA_COMPILERTESTCASE_H
#include <vector>
#include <TestCase/TestCaseBase.h>
#include <core/Containers/Array.h>
#include <core/compiler/Compiler.h>

namespace LXXTest
{


class CompilerTestCase : public TestCaseBase
{
    TEST_CASE_DECLARE( CompilerTestCase , TestCaseBase )

public:

    CompilerTestCase();
    virtual ~CompilerTestCase();

    bool Run() override;
private:
    Compiler *m_pCompiler;
    Array< const char* > m_scriptContents;

};


} // LXXTest

#endif //XLUA_COMPILERTESTCASE_H
