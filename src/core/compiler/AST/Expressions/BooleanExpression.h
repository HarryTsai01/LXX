//
// Created by Harry on 8/14/2023.
//

#ifndef XLUA_BOOLEANEXPRESSION_H
#define XLUA_BOOLEANEXPRESSION_H
#include "SimpleExpression.h"
namespace LXX {


class BooleanExpression : public SimpleExpression
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    BooleanExpression(bool value);

    bool GetValue() const { return _value; }
private:
    bool _value;
};


} // LXX

#endif //XLUA_BOOLEANEXPRESSION_H
