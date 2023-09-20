//
// Created by Harry on 8/28/2023.
//

#ifndef XLUA_EXPRESSIONLISTSTATEMENT_H
#define XLUA_EXPRESSIONLISTSTATEMENT_H
#include <core/compiler/AST/Expressions/ExpressionStatementBase.h>
#include <core/Containers/Array.h>

namespace LXX
{


class ExpressionListStatement : public ExpressionStatementBase
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    using ExpressionStatementBase::ExpressionStatementBase;
    void AddExpression( StatementBase* expression ) { _expressions.PushBack( expression ); }

    Array< StatementBase* >& GetExpressions() { return _expressions; }
    u32 GetExpressionCount() { return _expressions.Size(); }

private:
    Array< StatementBase* > _expressions;
};


} // LXX

#endif //XLUA_EXPRESSIONLISTSTATEMENT_H
