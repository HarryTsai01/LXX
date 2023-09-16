//
// Created by Harry on 9/17/2023.
//

#ifndef XLUA_LOG_H
#define XLUA_LOG_H
#include <utility>
namespace LXX
{
namespace LOG
{

enum class LogLevel
{
    Critical,
    Warning,
    Log,
    Display,
    Verbose,
};

enum class LogCategory
{
    LXX,
    Debugger,
    Interpreter,
};

template< typename ... Args>
void Logf( LogLevel level , LogCategory category , const char* format , Args ...args );

template< typename ... Args>
void LogCritical( LogCategory category , const char* format , Args ... args )
{
    Logf( LogLevel::Critical , category , format , std::forward<Args>(args)... );
}
template< typename ... Args>
void LogWarning( LogCategory category , const char* format , Args ... args )
{
    Logf( LogLevel::Warning , category , format , std::forward<Args>(args)... );
}
template< typename ... Args>
void Log( LogCategory category , const char* format , Args ... args )
{
    Logf( LogLevel::Log , category , format , std::forward<Args>(args)... );
}
template< typename ... Args>
void LogDisplay( LogCategory category , const char* format , Args ... args )
{
    Logf( LogLevel::Display , category , format , std::forward<Args>(args)... );
}
template< typename ... Args>
void LogVerbose( LogCategory category , const char* format , Args ... args )
{
    Logf( LogLevel::Verbose , category , format , std::forward<Args>(args)... );
}

} // LOG
} // LXX
#endif //XLUA_LOG_H
