//
// Created by Harry on 9/17/2023.
//

#ifndef XLUA_B2FCLOSECONNECTION_H
#define XLUA_B2FCLOSECONNECTION_H
#include "DebuggerProtocol.h"

namespace LXX
{
namespace Debugger
{
namespace Protocol
{


class B2FCloseConnection : public  Base
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    enum class Reason
    {
        FrontendVersionGreaterThanBackend
    };

    B2FCloseConnection( Reason reason )
    : Base( ProtocolType::B2F_CloseConnection )
    , _reason( reason )
    {

    }

    const char* GetReason() const;

private:
    Reason _reason;

};


} // Protocol
} // Debugger
} // LXX

#endif //XLUA_B2FCLOSECONNECTION_H
