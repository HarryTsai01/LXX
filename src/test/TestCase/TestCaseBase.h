//
// Created by Harry on 8/4/2023.
//

#ifndef XLUA_TESTCASEBASE_H
#define XLUA_TESTCASEBASE_H
#include <iostream>
#include <string>
#include <core/vm/VirtualMachine.h>

using namespace std;
using namespace LXX;

namespace LXXTest
{


class TestCaseBase
{
public:
    virtual bool Run() = 0;
    virtual string GetTestName() const = 0;
    virtual string GetErrorDetail() const {  return m_ErrorDetail; }

    bool Check( bool condition , const char* expression , const char* file , int line );
private:
    string m_ErrorDetail;
};

#define TEST_CASE_DECLARE( Class , Super ) \
    public:                                \
    string GetTestName() const override { return #Class; }

#define DO_TEST( condition ) \
        if( !Check( condition , #condition , __FILE__ , __LINE__ ) )  return false;
    }



#endif //XLUA_TESTCASEBASE_H
