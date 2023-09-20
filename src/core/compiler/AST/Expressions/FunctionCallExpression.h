//
// Created by Harry on 8/15/2023.
//

#ifndef XLUA_FUNCTIONCALLEXPRESSION_H
#define XLUA_FUNCTIONCALLEXPRESSION_H
#include "SimpleExpression.h"

namespace LXX
{


class FunctionCallExpression : public SimpleExpression
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    FunctionCallExpression( u32 lineNo );
    virtual ~FunctionCallExpression();

    void SetArguments( StatementBase* inArguments ) { _arguments = inArguments; }
    void SetVarExpression( StatementBase* inVarExpression ) { _varExpression = inVarExpression; }
    void SetFunctionName( StatementBase* inFunctionName ) { _functionName = inFunctionName; }

    StatementBase* GetArguments() { return _arguments; }
    StatementBase* GetVarExpression() { return _varExpression; }
    StatementBase* GetFunctionName() { return _functionName; }

    virtual void CollectReferences( GCObjectCollector& collector ) override;
private:
    StatementBase *_functionName;
    StatementBase *_varExpression;
    StatementBase *_arguments;
};


} // LXX

#endif //XLUA_FUNCTIONCALLEXPRESSION_H
