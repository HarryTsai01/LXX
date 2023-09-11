//
// Created by Harry on 8/4/2023.
//

#include "TestCaseBase.h"
#include <sstream>

namespace LXXTest
{


bool TestCaseBase::Check(bool condition, const char *expression, const char *file, int line)
{
    stringstream ss;
    if (! condition )
    {
        ss << expression << " test failed at " << file << ":" << line;
        m_ErrorDetail = ss.str();
    }
    return condition;
}


}