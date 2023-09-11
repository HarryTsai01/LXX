//
// Created by Harry on 8/27/2023.
//

#include "CompilerTestCase.h"
#include <core/exceptions/ExceptionBase.h>
#include <iostream>

namespace LXXTest
{


CompilerTestCase::CompilerTestCase()
{
    m_pCompiler = new Compiler();
    m_scriptContents =
            {
                    // return statement
                    "return \n",
                    "return 1\n",
                    "return 1,2\n",
                    "return 1,2,3\n",
                    // assignment statement
                    "a = 1",
                    "a , b = 1, 2",
                    // expression list
                    "a = true",
                    "a = false",
                    "a = 1.1",
                    "a = .123",
                    "a = 0123",
                    "a = 0b101",
                    "a = 0B101",
                    "a = 0x1f1f",
                    "a = 0X1f1f",
                    "a = nil",
                    "a = -1",
                    "a = not true",
                    "len = #tb",
                    "a = ~0b111",
                    "a = \"abc\"",
                    "a = 2^2",
                    "a = 1+1",
                    "a = 1*1",
                    "a = 1/1",
                    "a = 1//1",
                    "a = 1%1",
                    "a = c .. d",
                    "a = \"Hello,\" .. d",
                    "a = \"Hello,\" .. \'World!\'\n",
                    " a = 1 >> 10 ",
                    " a = 1 << 10 ",
                    " a = a << 10 ",
                    " a = a >> 10 ",
                    " a = a & b ",
                    " a = 0xff & 0xaaff ",
                    " a = a ~ b ",
                    " a = a | b ",
                    " a = 0xff | 1223 ",
                    " a = b < c ",
                    " a = b <= c ",
                    " a = b > c ",
                    " a = b >= c ",
                    " a = b ~= c ",
                    " a = b == c ",
                    " a = b == c and e > f or g >=  h ",
                    // table constructor
                    "a = {}",
                    "a = { age = 10 , name = \"Tom\" }",
                    "a = { [\"age\"] = 10 , [\"name\"] = \"Tom\" }",
                    "a = { 1 , 2 , 3 , }",
                    "a = { 1 , 2 , 3 , age = 10 , [\"name\"] = \"Tom\" }",
                    // long string
                    "a = [[this is a long string]]",
                    "a = [[\n"
                    "this is a long string\n"
                    "this is a long string\n"
                    "this is a long string\n"
                    "]]\n",
                    "a = [=[\n"
                    "this is a long string\n"
                    "this is a long string\n"
                    "this is a long string\n"
                    "]=]\n",
                    // function call
                    "foo()",
                    "foo(...)",
                    "foo(1,...)",
                    "foo \"Hello,World!\"",
                    "foo { age = 10 , name = \'Tom\' }",
                    "foo(\"Hello,World!\")",
                    "foo(\"Hello,World!\", { age = 10 , name = \'Tom\' })",
                    "tb1.foo(1,2,3)",
                    "tb1:foo(1,2,3)",
                    "tb2.tb1:foo(1,2,3)",
                    // while statement
                    "while true do end",
                    "local a = 1\n"
                    "while a < 10 do\n"
                    "   print(a)\n"
                    "   a = a + 1\n"
                    "end",
                    // repeat statement
                    "local a = 1\n"
                    "repeat\n"
                    "   print(a)\n"
                    "   a = a + 1\n"
                    "until a > 10\n",
                    // for statement
                    "for i = 1, 10 do\n"
                    "   if i % 2 == 0 then\n"
                    "       print(i)\n"
                    "       break\n"
                    "   else\n"
                    "       continue\n"
                    "   end\n"
                    "end",
                    "for i = 10, 1, -1 do\n"
                    "   print(i)\n"
                    "end",
                    "for k, v in pairs(tb) do\n"
                    "   print(k, v)\n"
                    "end",
                    // do end block statement
                    "do\n"
                    "   print(\"Hello,World!\")\n"
                    "end",
                    // if statement
                    "if true then\n"
                    "   print(\"Hello,World!\")\n"
                    "end",
                    "if a == 1 then\n"
                    "   print(\" a is equal to 1\")\n"
                    "elseif a == 2 then\n"
                    "   print(\" a is equal to 2\")\n"
                    "else\n"
                    "   print(\" a is not equal to 1 or 2\")\n"
                    "end",
                    // comment
                    "-- this is a comment\n",
                    "--[[  this is a comment --]]\n",
                    // function definition
                    "function foo() \n"
                    "   print(\"Hello,World!\")\n "
                    "end",
                    "function add( a , b ) \n"
                    "   return a + b\n "
                    "end",
                    "function tb.add( a , b ) \n"
                    "   return a + b\n "
                    "end",
                    "function ui.widget:add( child ) \n"
                    "   return self:AddChild( child )\n "
                    "end",
                    // local statement
                    "local a = 1",
                    "local b = 2",
                    "local c, d = 3, 4",
            };
}


CompilerTestCase::~CompilerTestCase()
{
    delete m_pCompiler;
}


bool CompilerTestCase::Run()
{
    VirtualMachine vm;
    if( vm.Startup() )
    {
        for( auto scriptContent : m_scriptContents )
        {
            try
            {
                if( vm.CompileString( scriptContent ) == nullptr )
                    return false;
            }
            catch( ExceptionBase &e )
            {
                cerr << e.ToString() << endl;
                return false;
            }

        }
        vm.Shutdown();
        return true;
    }
    return false;

}


} // LXXTest