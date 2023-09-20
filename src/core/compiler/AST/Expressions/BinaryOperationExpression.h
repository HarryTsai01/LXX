//
// Created by Harry on 8/15/2023.
//

#ifndef XLUA_BINARYOPERATIONEXPRESSION_H
#define XLUA_BINARYOPERATIONEXPRESSION_H
#include "ExpressionStatementBase.h"

namespace LXX {


class BinaryOperationExpression : public ExpressionStatementBase
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
public:
    BinaryOperationExpression( u32 lineNo , s32 op , StatementBase* left, StatementBase* right );
    virtual ~BinaryOperationExpression();

    virtual void CollectReferences( GCObjectCollector& collector ) override;

    StatementBase* GetLeft() { return _left; }
    StatementBase* GetRight() { return _right; }

    s32 GetOperator() { return _operator; }
private:
    StatementBase* _left;
    StatementBase* _right;
    s32 _operator;
};


} // LXX

#endif //XLUA_BINARYOPERATIONEXPRESSION_H
