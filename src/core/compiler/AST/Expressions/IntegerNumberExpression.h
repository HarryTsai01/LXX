//
// Created by Harry on 8/14/2023.
//

#ifndef XLUA_INTEGERNUMBEREXPRESSION_H
#define XLUA_INTEGERNUMBEREXPRESSION_H
#include "SimpleExpression.h"

namespace LXX {


class IntegerNumberExpression : public SimpleExpression
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    IntegerNumberExpression( u32 lineNo , s32 value );

    s32 GetValue() const { return _value; }
private:
    s32 _value;
};


} // LXX

#endif //XLUA_INTEGERNUMBEREXPRESSION_H
