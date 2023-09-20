//
// Created by Harry on 8/16/2023.
//

#ifndef XLUA_IDENTIFIEREXPRESSION_H
#define XLUA_IDENTIFIEREXPRESSION_H
#include "SimpleExpression.h"
#include <core/objects/string/String.h>

namespace LXX
{


class IdentifierExpression : public SimpleExpression
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    IdentifierExpression( u32 lineNo , String* identifier );

    virtual void CollectReferences( GCObjectCollector &Collector ) override;

    String* GetIdentifier() const { return _identifier; }
private:
    String* _identifier;
};


} // LXX

#endif //XLUA_IDENTIFIEREXPRESSION_H
