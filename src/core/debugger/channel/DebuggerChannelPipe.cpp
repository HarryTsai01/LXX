//
// Created by Harry on 9/16/2023.
//

#include "DebuggerChannelPipe.h"
#include <core/debugger/protocol/DebuggerProtocol.h>
#include <core/archive/MemoryArchiveWriter.h>
#include <core/archive/MemoryArchiveReader.h>
#include <core/archive/iostream/IOStream.h>
#include <core/config/Config.h>

namespace LXX
{
namespace Debugger
{

ChannelPipe::ChannelPipe()
 : _pipeName( nullptr )
{

}


ChannelPipe::~ChannelPipe()
{

}


bool ChannelPipe::Initialize( )
{
    return true;
}


void ChannelPipe::Close()
{

}


bool ChannelPipe::Send( Protocol::Base* protocol )
{
    MemoryArchiveWriter archive;
    IOStreamBE stream( archive );
    stream << protocol;
    return _pipeInst.Write( archive.GetBuffer(), archive.GetBufferSize( ) );
}


bool ChannelPipe::Receive( Protocol::Base* protocol )
{
    u8 _buffer[ Config::DEBUGGER_MAX_PROTOCOL_SIZE];
    u32 _receiveSize = Config::DEBUGGER_MAX_PROTOCOL_SIZE;
    if( !_pipeInst.Read( _buffer, _receiveSize ) )
    {
        MemoryArchiveReader archive(  _buffer, _receiveSize );
        IOStreamBE stream( archive );
        stream >> protocol;

        return true;
    }

    return false;
}


} // Debugger
} // LXX