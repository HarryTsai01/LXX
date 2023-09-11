//
// Created by Harry on 9/11/2023.
//

#include "MetaTableMgr.h"
#include "MetaMethodHandlerBoolean.h"
#include "MetaMethodHandlerFullUserdata.h"
#include "MetaMethodHandlerFunction.h"
#include "MetaMethodHandlerLightUserdata.h"
#include "MetaMethodHandlerNil.h"
#include "MetaMethodHandlerNumber.h"
#include "MetaMethodHandlerString.h"
#include "MetaMethodHandlerTable.h"
#include "MetaMethodHandlerThread.h"

namespace LXX
{


MetaTableMgr*  MetaTableMgr::_sInst = nullptr;

MetaTableMgr::MetaTableMgr()
{
    _metaMethodMap =
    {
            { ValueType::Boolean, new MetaMethodHandlerBoolean() },
            { ValueType::FullUserdata, new MetaMethodHandlerFullUserdata() },
            { ValueType::Function, new MetaMethodHandlerFunction() },
            { ValueType::LightUserdata, new MetaMethodHandlerLightUserdata() },
            { ValueType::Nil, new MetaMethodHandlerNil() },
            { ValueType::Number, new MetaMethodHandlerNumber() },
            { ValueType::String, new MetaMethodHandlerString() },
            { ValueType::Table, new MetaMethodHandlerTable() },
            { ValueType::Thread, new MetaMethodHandlerThread() },
    };
}

MetaTableMgr& MetaTableMgr::GetInstance()
{
    if( _sInst == nullptr ) _sInst = new MetaTableMgr();
    return *_sInst;
}


MetaMethodHandler* MetaTableMgr::GetMetaMethodHandler( ValueType valueType )
{
    auto it = _metaMethodMap.Find( valueType );
    if(  it != _metaMethodMap.End() )
        return it->Second;
    return nullptr;
}


}