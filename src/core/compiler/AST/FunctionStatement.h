//
// Created by Harry on 8/11/2023.
//

#ifndef XLUA_FUNCTIONSTATEMENT_H
#define XLUA_FUNCTIONSTATEMENT_H
#include "StatementBase.h"
#include <core/Containers/Array.h>
#include <core/compiler/AST/BlockStatement.h>
#include <core/objects/string/String.h>

namespace LXX {


class FunctionStatement : public StatementBase
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    FunctionStatement( u32 lineNo );
    virtual ~FunctionStatement();

    virtual void CollectReferences( GCObjectCollector &collector ) override;

    void SetFunctionName( StatementBase* functionName );
    void SetFunctionBody( StatementBase* body );
    void AddParameter( StatementBase* parameter );
    void AddSelfParameter();

    StatementBase* GetFunctionName() const { return _functionName; }
    StatementBase* GetFunctionBody() const { return _body; }
    u32 GetParameterCount() const ;
    Array< StatementBase* > GetParameters() const { return _parameters; }

    Array< String* > FetchParameterNames() const;
private:
    StatementBase* _functionName;
    StatementBase* _body;
    Array< StatementBase* > _parameters;

};


} // LXX

#endif //XLUA_FUNCTIONSTATEMENT_H
