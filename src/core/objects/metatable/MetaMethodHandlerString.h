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
    MetaMethodHandlerString();
    DECLARE_META_METHOD( META_METHOD_KEY_BIN_OP_ADD )
    DECLARE_META_METHOD( META_METHOD_KEY_BIN_OP_CONCAT )
    DECLARE_META_METHOD( META_METHOD_KEY_BIN_OP_EQUAL )
    DECLARE_META_METHOD( META_METHOD_KEY_BIN_OP_LESS_THAN )
    DECLARE_META_METHOD( META_METHOD_KEY_BIN_OP_LESS_THAN_OR_EQUAL )
};


} // LXX

#endif //XLUA_METAMETHODHANDLERSTRING_H
