//
// Created by Harry on 9/11/2023.
//

#ifndef XLUA_METATABLEMGR_H
#define XLUA_METATABLEMGR_H
#include <core/LXX.h>
#include <core/mem/MemoryAllocator.h>
#include <core/Value.h>
#include <core/containers/UnorderedMap.h>
#include "MetaMethodHandler.h"

namespace LXX
{


class MetaTableMgr
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    static MetaTableMgr& GetInstance();
private:
    MetaTableMgr();
private:
    static MetaTableMgr* _sInst;
    UnorderedMap< ValueType, MetaMethodHandler*> _metaMethodMap;
};


}



#endif //XLUA_METATABLEMGR_H
