//
// Created by Harry on 9/11/2023.
//

#ifndef XLUA_METAMETHODHANDLERNUMBER_H
#define XLUA_METAMETHODHANDLERNUMBER_H
#include "MetaMethodHandler.h"

namespace LXX {

class MetaMethodHandlerNumber :  public MetaMethodHandler
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    MetaMethodHandlerNumber() = default;
};

} // LXX

#endif //XLUA_METAMETHODHANDLERNUMBER_H
