//
// Created by Harry on 8/11/2023.
//

#ifndef XLUA_EXPRESSIONSTATEMENT_H
#define XLUA_EXPRESSIONSTATEMENT_H
#include <core/compiler/AST/StatementBase.h>

namespace LXX
{


class ExpressionStatementBase : public StatementBase
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    using StatementBase::StatementBase;
};


} // LXX

#endif //XLUA_EXPRESSIONSTATEMENT_H
