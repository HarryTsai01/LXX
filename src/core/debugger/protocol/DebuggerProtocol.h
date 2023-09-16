//
// Created by Harry on 9/16/2023.
//

#ifndef XLUA_DEBUGGERPROTOCOL_H
#define XLUA_DEBUGGERPROTOCOL_H
#include <core/archive/iostream/IOStream.h>
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

class Base
{
public:
    Base() = default;
    virtual ~Base() = default;

    virtual void Serialize( IOStreamBase &stream ) = 0;

    template< bool BigEndian>
    friend IOStream<BigEndian> & operator <<( IOStream<BigEndian> &stream, Base *base );

    template< bool BigEndian>
    friend IOStream<BigEndian> & operator >>( IOStream<BigEndian> &stream, Base *&base );
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


} // Protocol
} // Debugger
} // LXX

#endif //XLUA_DEBUGGERPROTOCOL_H
