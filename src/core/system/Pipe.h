//
// Created by Harry on 9/16/2023.
//

#ifndef XLUA_PIPE_H
#define XLUA_PIPE_H
#include <core/LXX.h>
#include <core/objects/string/string.h>

namespace LXX
{
namespace System {


class Pipe
{
public:
    Pipe();
    ~Pipe();
    bool Create( String *pipeName, u32 buffSize );
    void Destroy();

private:
    u64 _pipeHandle;
};

class PipeInstance
{
public:
    PipeInstance();
    ~PipeInstance();

    bool Create( String *pipeName );
    void Destroy();
    bool Write( const u8 *data, u32 size);
    bool Read(u8 *data, u32 &size);

private:
    u64 _pipeHandle;
};


} // System
} // LXX

#endif //XLUA_PIPE_H
