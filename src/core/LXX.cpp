//
// Created by Harry on 9/17/2023.
//
#include "LXX.h"
#include <core/archive/iostream/iostream.h>

namespace LXX
{


Version GLXXVersion { 1,0,0,0 };
Version GLXXDebuggerVersion { 1 ,0 , 0 , 0 };

IOStreamBase& operator<<( IOStreamBase &stream , Version& version)
{
    stream << version._major;
    stream << version._minor;
    stream << version._build;
    stream << version._revision;
    return stream;
}
IOStreamBase& operator>>( IOStreamBase &stream , Version& version)
{
    stream >> version._major;
    stream >> version._minor;
    stream >> version._build;
    stream >> version._revision;
    return stream;
}
}
