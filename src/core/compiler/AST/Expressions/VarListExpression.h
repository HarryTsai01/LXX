//
// Created by Harry on 8/31/2023.
//

#ifndef XLUA_VARLISTEXPRESSION_H
#define XLUA_VARLISTEXPRESSION_H
#include "ExpressionStatementBase.h"
#include "VarExpression.h"
#include <core/Containers/Array.h>

namespace LXX
{


class VarListExpression : public ExpressionStatementBase
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    VarListExpression() = default;

    void AddVarExpression( VarExpression* varExpression ) { _varExpressions.Add( varExpression ); }
    Array< VarExpression* >& GetVarExpressions() { return _varExpressions; }
    u32 GetVarExpressionCount() { return _varExpressions.Size(); }
private:
    Array< VarExpression* > _varExpressions;
};

} // LXX

#endif //XLUA_VARLISTEXPRESSION_H
