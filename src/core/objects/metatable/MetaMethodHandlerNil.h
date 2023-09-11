//
// Created by Harry on 9/11/2023.
//

#ifndef XLUA_METAMETHODHANDLERNIL_H
#define XLUA_METAMETHODHANDLERNIL_H
#include "MetaMethodHandler.h"

namespace LXX {


class MetaMethodHandlerNil :  public MetaMethodHandler
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    MetaMethodHandlerNil() = default;
};


} // LXX

#endif //XLUA_METAMETHODHANDLERNIL_H
