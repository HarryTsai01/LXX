//
// Created by Harry on 8/14/2023.
//

#ifndef XLUA_REALNUMBEREXPRESSION_H
#define XLUA_REALNUMBEREXPRESSION_H
#include "SimpleExpression.h"

namespace LXX {


class RealNumberExpression : public SimpleExpression
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    RealNumberExpression( f64 value );

    f64 GetValue() const { return _value; }
private:
    f64 _value;
};


} // LXX

#endif //XLUA_REALNUMBEREXPRESSION_H
