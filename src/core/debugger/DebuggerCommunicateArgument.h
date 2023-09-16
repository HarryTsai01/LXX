//
// Created by Harry on 9/16/2023.
//

#ifndef XLUA_DEBUGGERCOMMUNICATEARGUMENT_H
#define XLUA_DEBUGGERCOMMUNICATEARGUMENT_H
#include <core/objects/string/string.h>

namespace LXX
{
namespace Debugger
{

class BindingArgument
{
public:
    BindingArgument() = default;
    virtual ~BindingArgument() = default;
};


class BindingArgumentSocket : public BindingArgument
{
public:
    BindingArgumentSocket( u16 port )
    : _port( port )
    {

    }
    virtual ~BindingArgumentSocket() = default;

    u16 GetPort() const { return _port; }
private:
    u16 _port;
};


class BindingArgumentPipe : public BindingArgument
{
public:
    BindingArgumentPipe( String* pipeName )
    : _pipeName( pipeName )
    {

    }
    virtual ~BindingArgumentPipe() = default;

    String* GetPipeName() const { return _pipeName; }
private:
    String *_pipeName;
};


class ConnectArgument
{
public:
    ConnectArgument() = default;
    virtual ~ConnectArgument() = default;
};


class ConnectArgumentSocket : public ConnectArgument
{
public:
    ConnectArgumentSocket( String* hostIp, u16 port )
    : _hostIp( hostIp )
    , _port( port )
    {

    }
    virtual ~ConnectArgumentSocket() = default;

    String* GetHostIp() const { return _hostIp; }
    u16 GetPort() const { return _port; }
private:
    String* _hostIp;
    u16 _port;
};


class ConnectArgumentPipe : public ConnectArgument
{
public:
    ConnectArgumentPipe( String* pipeName )
    : _pipeName( pipeName )
    {

    }
    virtual ~ConnectArgumentPipe() = default;
    String* GetPipeName() const { return _pipeName; }
private:
    String *_pipeName;
};


}
}

#endif //XLUA_DEBUGGERCOMMUNICATEARGUMENT_H
