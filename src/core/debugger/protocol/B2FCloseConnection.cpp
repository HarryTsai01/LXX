//
// Created by Harry on 9/17/2023.
//

#include "B2FCloseConnection.h"
#include <core/containers/UnorderedMap.h>

namespace LXX
{
namespace Debugger
{
namespace Protocol
{


const char *B2FCloseConnection::GetReason() const
{
    static UnorderedMap< Reason , const char* > reasonMap = {
            { Reason::FrontendVersionGreaterThanBackend , "The frontend version is newer than backend version , please update backend to date." },
    };

    return reasonMap[_reason];
}


} // Protocol
} // Debugger
} // LXX