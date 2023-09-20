//
// Created by Harry on 8/28/2023.
//

#ifndef XLUA_UNARYOPERATOREXPRESSION_H
#define XLUA_UNARYOPERATOREXPRESSION_H
#include "ExpressionStatementBase.h"

namespace LXX
{

class UnaryOperatorExpression : public ExpressionStatementBase
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    UnaryOperatorExpression( u32 lineNo , s32 tokenType , StatementBase* expression );

    s32 GetTokenType() const { return  _tokenType; }
    StatementBase* GetExpression() const { return _expression; }
private:
    s32 _tokenType;
    StatementBase* _expression;
};

} // LXX

#endif //XLUA_UNARYOPERATOREXPRESSION_H
