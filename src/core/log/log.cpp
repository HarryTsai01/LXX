//
// Created by Harry on 9/17/2023.
//
#include "log.h"
#include <iostream>
#include <core/containers/UnorderedMap.h>

namespace LXX
{
namespace LOG
{


EventOnPreLogEvent GOnPreLogEvent;
EventOnPostLogEvent GOnPostLogEvent;
LogLevel GLogLevel = LogLevel::Verbose;

static UnorderedMap< LogLevel , const char* > logLevelToNames =
{
        { LogLevel::Critical , "Critical" } ,
        { LogLevel::Warning , "Warning" } ,
        { LogLevel::Log , "Log" } ,
        { LogLevel::Display , "Display" } ,
        { LogLevel::Verbose , "Verbose" } ,
};

static UnorderedMap< LogCategory , const char* > logCategoryToNames =
{
        { LogCategory::LXX , "LXX" } ,
        { LogCategory::Debugger , "Debugger" } ,
        { LogCategory::Interpreter , "Interpreter" } ,
        { LogCategory::StateMachine , "StateMachine" } ,
};


const char* GetLogLevelName( LogLevel logLevel )
{
    auto It = logLevelToNames.Find( logLevel );
    assert( It != logLevelToNames.End() );
    return It->Second;
}


const char* GetLogCategoryName( LogCategory logCategory )
{
    auto It = logCategoryToNames.Find( logCategory ) ;
    assert( It != logCategoryToNames.End() );
    return It->Second;
}

void SetLogLevel( LogLevel logLevel )
{
    GLogLevel = logLevel;
}


LogLevel GetLogLevel()
{
    return GLogLevel;
}

}
}