//
// Created by Harry on 9/11/2023.
//

#ifndef XLUA_METAMETHODHANDLERBOOLEAN_H
#define XLUA_METAMETHODHANDLERBOOLEAN_H
#include "MetaMethodHandler.h"

namespace LXX
{


class MetaMethodHandlerBoolean : public MetaMethodHandler
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    MetaMethodHandlerBoolean();
    DECLARE_META_METHOD( META_METHOD_KEY_BIN_OP_EQUAL )
};


} // LXX

#endif //XLUA_METAMETHODHANDLERBOOLEAN_H
