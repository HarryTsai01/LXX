//
// Created by Harry on 9/11/2023.
//

#ifndef XLUA_METAMETHODHANDLERTHREAD_H
#define XLUA_METAMETHODHANDLERTHREAD_H
#include "MetaMethodHandler.h"

namespace LXX
{


class MetaMethodHandlerThread :  public MetaMethodHandler
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    MetaMethodHandlerThread() = default;
};


} // LXX

#endif //XLUA_METAMETHODHANDLERTHREAD_H
