//
// Created by Harry on 8/11/2023.
//

#ifndef XLUA_STATEMENTBASE_H
#define XLUA_STATEMENTBASE_H
#include <core/LXX.h>
#include <core/mem/MemoryAllocator.h>
#include <core/objects/GCObject.h>
#include <core/objects/GCObjectCollector.h>

namespace LXX
{


class StatementBase : public GCObject
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    StatementBase( u32 lineNo );
    virtual ~StatementBase();

    virtual void CollectReferences( GCObjectCollector& collector ) override;

    u32 GetLineNo() const { return _lineNo ; }
protected:
    u32 _lineNo;
};


} // LXX

#endif //XLUA_STATEMENTBASE_H
