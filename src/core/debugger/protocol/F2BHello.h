//
// Created by Harry on 9/16/2023.
//

#ifndef XLUA_DEBUGGERPROTOCOLF2BHELLO_H
#define XLUA_DEBUGGERPROTOCOLF2BHELLO_H
#include "DebuggerProtocol.h"

namespace LXX
{
namespace Debugger
{
namespace Protocol
{


class F2BHello : public Base
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    using Base::Base;

    void Serialize( IOStreamBase &stream ) ;
private:
    Version _version;
};


}

}


} // LXX

#endif //XLUA_DEBUGGERPROTOCOLF2BHELLO_H
