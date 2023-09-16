//
// Created by Harry on 9/16/2023.
//

#ifndef XLUA_DEBUGGERPROTOCOLFACTORY_H
#define XLUA_DEBUGGERPROTOCOLFACTORY_H
#include <core/Singleton.h>
#include <core/containers/UnorderedMap.h>

namespace LXX
{
namespace Debugger
{
namespace Protocol
{
class Base;
class Factory : public Singleton<Factory>
{
    friend class Singleton<Factory>;
public:
    Base* Create( s32 protocolType );

    template<typename T>
    Base* Create( T protocolType )
    {
        return Create( static_cast<s32>(protocolType) );
    }
private:
    Factory();
    Factory(const Factory&) = delete;
    Factory(Factory&&) = delete;
    Factory& operator=(const Factory&) = delete;
    Factory& operator=(Factory&&) = delete;
private:
    class Proxy
    {
    public:
        Proxy() = default;
        virtual ~Proxy() = default;
        virtual Base* Create() = 0;
    };
    template<typename T , s32 protocolType>
    class ProxyImpl : public Proxy
    {
    public:
        using Proxy::Proxy;
        virtual Base* Create() override;
    };

private:
    UnorderedMap<s32, Proxy*> _protocols;

};


} // Protocol
} // Debugger
} // LXX

#endif //XLUA_DEBUGGERPROTOCOLFACTORY_H
