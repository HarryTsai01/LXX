//
// Created by Harry on 9/16/2023.
//
#include "Pipe.h"

#if PLATFORM_IS_WINDOWS
#include <Windows.h>
#endif

namespace LXX
{
namespace System
{


Pipe::Pipe()
: _pipeHandle( -1 )
{

}


Pipe::~Pipe()
{
    Destroy();
}


bool Pipe::Create( String *pipeName, u32 buffSize )
{
#if PLATFORM_IS_WINDOWS
    HANDLE hPipe = CreateNamedPipeA(
            pipeName->GetData(),  // pipe name
            PIPE_ACCESS_DUPLEX,  // read/write access
            PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,  // byte-type pipe
            1,  // maximum instances
            buffSize,  // output buffer size
            buffSize,  // input buffer size
            0,  // time-out interval
            NULL);  // default security attribute

    _pipeHandle = reinterpret_cast<u64>(hPipe);
    return _pipeHandle != -1;
#endif
    return false;
}


void Pipe::Destroy()
{
    if ( _pipeHandle != -1 )
    {
#if PLATFORM_IS_WINDOWS
        CloseHandle( reinterpret_cast<HANDLE>( _pipeHandle ) );
#endif
        _pipeHandle = -1;
    }
}


PipeInstance::PipeInstance()
: _pipeHandle( -1 )
{

}


PipeInstance::~PipeInstance()
{
    Destroy();
}


bool PipeInstance::Create( String *pipeName )
{
#if PLATFORM_IS_WINDOWS
    HANDLE hPipe = CreateFileA(
            pipeName->GetData(),  // pipe name
            GENERIC_READ | GENERIC_WRITE,  // read/write access
            0,  // no sharing
            NULL,  // default security attributes
            OPEN_EXISTING,  // opens existing pipe
            0,  // default attributes
            NULL);  // no template file
    _pipeHandle = reinterpret_cast<u64>(hPipe);
    return _pipeHandle != -1;
#endif

    return false;
}


void PipeInstance::Destroy()
{
    if ( _pipeHandle != -1 )
    {
#if PLATFORM_IS_WINDOWS
        CloseHandle( reinterpret_cast<HANDLE>( _pipeHandle ) );
#endif
        _pipeHandle = -1;
    }
}


bool PipeInstance::Write( const  u8 *data, u32 size )
{
    if ( _pipeHandle != -1 )
    {
#if PLATFORM_IS_WINDOWS
        DWORD bytesWritten;
        WriteFile( reinterpret_cast<HANDLE>( _pipeHandle ), data, size, &bytesWritten, NULL );
        return bytesWritten == size;
#endif
    }
    return false;
}


bool PipeInstance::Read( u8 *data, u32 &size )
{
    if ( _pipeHandle != -1 )
    {
#if PLATFORM_IS_WINDOWS
        DWORD bytesRead;
        ReadFile( reinterpret_cast<HANDLE>( _pipeHandle ), data, size, &bytesRead, NULL );
        size = bytesRead;
        return true;
#endif

    }
    return false;
}

} // System
} // LXX