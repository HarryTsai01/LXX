//
// Created by Harry on 9/11/2023.
//

#ifndef XLUA_METAMETHODHANDLERTABLE_H
#define XLUA_METAMETHODHANDLERTABLE_H
#include "MetaMethodHandler.h"

namespace LXX
{


class MetaMethodHandlerTable :  public MetaMethodHandler
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    MetaMethodHandlerTable() = default;
};


} // LXX

#endif //XLUA_METAMETHODHANDLERTABLE_H
