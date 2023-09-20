//
// Created by Harry on 8/29/2023.
//

#ifndef XLUA_VAREXPRESSION_H
#define XLUA_VAREXPRESSION_H
#include "ExpressionStatementBase.h"
#include "IdentifierExpression.h"
#include <core/Containers/Array.h>

namespace LXX {


class VarExpression : public  ExpressionStatementBase
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    VarExpression( u32 lineNo , StatementBase* statement ) ;

    void AddStatement( StatementBase* statement ) { _statements.Add( statement ); }
    Array< StatementBase* >& GetStatements() { return _statements; }
private:
    Array< StatementBase* > _statements;
};


} // LXX

#endif //XLUA_VAREXPRESSION_H
