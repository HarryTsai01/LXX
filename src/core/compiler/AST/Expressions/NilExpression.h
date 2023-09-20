//
// Created by Harry on 8/14/2023.
//

#ifndef XLUA_NILEXPRESSION_H
#define XLUA_NILEXPRESSION_H
#include "SimpleExpression.h"

namespace LXX {


class NilExpression : public SimpleExpression
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    using SimpleExpression::SimpleExpression;
};


} // LXX

#endif //XLUA_NILEXPRESSION_H
