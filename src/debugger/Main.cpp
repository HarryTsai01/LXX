//
// Created by Harry on 9/17/2023.
//
#include <core/LXX.h>
#include <core/log/log.h>
#include <iostream>

using namespace LXX;
using namespace LXX::LOG;

void PrintVersionInfo()
{
    Log( LogCategory::Debugger , " Welcome to use LXX debugger, current version is %d.%d.%d.%d"
             , GLXXDebuggerVersion._major
            , GLXXDebuggerVersion._minor
            , GLXXDebuggerVersion._build
            , GLXXDebuggerVersion._revision
         );
}

int main( int argc , const char ** argv )
{
    PrintVersionInfo();
    std::cout << ">>>";
    while( true )
    {
        char buff[1024] = { 0 };
        std::cin >> buff;
        std::cout << buff << std::endl;
        std::cout << ">>>";
    }
    return 0;
}