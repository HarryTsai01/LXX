//
// Created by Harry on 9/11/2023.
//

#ifndef XLUA_METAMETHODHANDLERSTRING_H
#define XLUA_METAMETHODHANDLERSTRING_H
#include "MetaMethodHandler.h"

namespace LXX
{


class MetaMethodHandlerString :  public MetaMethodHandler
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    MetaMethodHandlerString() = default;
};


} // LXX

#endif //XLUA_METAMETHODHANDLERSTRING_H
