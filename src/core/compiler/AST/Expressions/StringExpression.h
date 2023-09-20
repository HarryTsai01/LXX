//
// Created by Harry on 8/14/2023.
//

#ifndef XLUA_STRINGEXPRESSION_H
#define XLUA_STRINGEXPRESSION_H
#include "SimpleExpression.h"
#include <core/objects/string/string.h>

namespace LXX
{


class StringExpression : public SimpleExpression
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    StringExpression( u32 lineNo , String* value );
    virtual ~StringExpression();
    virtual void CollectReferences( GCObjectCollector &collector ) override;

    String* GetValue() const { return _value; }

private:
    String* _value;

};


} // LXX

#endif //XLUA_STRINGEXPRESSION_H
