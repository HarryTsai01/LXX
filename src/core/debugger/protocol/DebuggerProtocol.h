//
// Created by Harry on 9/16/2023.
//

#ifndef XLUA_DEBUGGERPROTOCOL_H
#define XLUA_DEBUGGERPROTOCOL_H
#include <core/archive/iostream/IOStream.h>
#include <core/objects/GCObject.h>
#include "DebuggerProtocolFactory.h"

namespace LXX
{
namespace Debugger
{
namespace Protocol
{

enum class ProtocolType
{
    // >>> the protocol from frontend to backend
    F2B_Start,
    F2B_Hello,
    F2B_Max,
    // <<< the protocol from backend to frontend

    // >> the protocol from frontend to backend
    B2F_Start,
    Max,
    // << the protocol from backend to frontend
};

IOStreamBase& operator <<( IOStreamBase &stream, ProtocolType& type )
{
    return stream << reinterpret_cast<s32&>( type );
}
IOStreamBase& operator >> ( IOStreamBase &stream, ProtocolType& type )
{
    return stream >> reinterpret_cast<s32&>( type );
}

class Base : public GCObject
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    Base( ProtocolType protocolType )
        : _protocolType( protocolType )
    {

    }
    virtual ~Base() = default;

    virtual void Serialize( IOStreamBase &stream );

    template< bool BigEndian>
    friend IOStream<BigEndian> & operator <<( IOStream<BigEndian> &stream, Base *base );

    template< bool BigEndian>
    friend IOStream<BigEndian> & operator >>( IOStream<BigEndian> &stream, Base *&base );
protected:
    ProtocolType _protocolType;
}; // Base

template< bool BigEndian>
IOStream<BigEndian> & operator <<( IOStream<BigEndian> &stream, Base *base )
{
    base->Serialize( stream );
    return stream;
}


template< bool BigEndian>
IOStream<BigEndian> & operator >>( IOStream<BigEndian> &stream, Base *&base )
{
    ProtocolType type;
    stream >> type;
    base = Factory::GetInstance().Create( type );
    return stream;
}


class ProtocolHandler
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    ProtocolHandler() = default;
    virtual ~ProtocolHandler() = default;
    virtual void Invoke( Base* protocol ) = 0 ;
};

template< typename ProtocolClass , typename Class , typename CallbackFunction>
class ProtocolHandlerImpl : public ProtocolHandler
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    ProtocolHandlerImpl( Class* inst , CallbackFunction inCallback )
        : _inst( inst )
        , _callback( inCallback )
    {

    }
    virtual ~ProtocolHandlerImpl() = default;

    virtual void Invoke( Base *protocol ) override
    {
        (_inst->*_callback)( dynamic_cast< ProtocolClass* >( protocol ) );
    }

private:
    Class *_inst;
    CallbackFunction _callback;
};

} // Protocol
} // Debugger
} // LXX

#endif //XLUA_DEBUGGERPROTOCOL_H
