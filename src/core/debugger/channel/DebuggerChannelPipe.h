//
// Created by Harry on 9/16/2023.
//

#ifndef XLUA_DEBUGGERCHANNELPIPE_H
#define XLUA_DEBUGGERCHANNELPIPE_H
#include "DebuggerChannel.h"
#include <core/LXX.h>
#include <core/mem/MemoryAllocator.h>
#include <core/system/pipe.h>

namespace LXX
{
namespace Debugger
{


class ChannelPipe : public Channel
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    ChannelPipe();
    virtual ~ChannelPipe();
    virtual bool Initialize() override;
    virtual void Close() override;
    virtual bool Send( Protocol::Base* protocol ) override;
    virtual bool Receive( Protocol::Base* protocol ) override;
protected:
    String *_pipeName;
    System::PipeInstance _pipeInst;
}; // ChannelPipe


class ChannelPipeServer : public ChannelPipe
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    ChannelPipeServer();
    virtual ~ChannelPipeServer() ;

    virtual bool Bind( BindingArgument* argument ) override;
    virtual bool Listen() override;
    virtual Channel *Accept() override;
private:
    System::Pipe _pipe;
};


class ChannelPipeClient : public ChannelPipe
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    virtual bool Connect( ConnectArgument* argument ) override;
};


}// Debugger
} // LXX

#endif //XLUA_DEBUGGERCHANNELPIPE_H
