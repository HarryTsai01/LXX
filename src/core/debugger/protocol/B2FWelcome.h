//
// Created by Harry on 9/17/2023.
//

#ifndef XLUA_B2FWELCOME_H
#define XLUA_B2FWELCOME_H
#include "DebuggerProtocol.h"

namespace LXX
{
namespace Debugger
{
namespace Protocol
{

class B2FWelcome : public Base
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    B2FWelcome()
     : Base( ProtocolType::B2F_Welcome )
     , _version( GLXXDebuggerVersion )
    {

    }

    Version GetVersion() const { return _version ; }
private:
    Version _version;
}; //B2FWelcome
} //Protocol
} // Debugger
} // LXX

#endif //XLUA_B2FWELCOME_H
