//
// Created by Harry on 9/11/2023.
//

#ifndef XLUA_METAMETHODHANDLERLIGHTUSERDATA_H
#define XLUA_METAMETHODHANDLERLIGHTUSERDATA_H
#include "MetaMethodHandler.h"

namespace LXX
{


class MetaMethodHandlerLightUserdata : public MetaMethodHandler
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    MetaMethodHandlerLightUserdata() = default;
};


} // LXX

#endif //XLUA_METAMETHODHANDLERLIGHTUSERDATA_H
