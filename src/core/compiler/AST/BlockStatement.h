//
// Created by Harry on 8/28/2023.
//

#ifndef XLUA_BLOCKSTATEMENT_H
#define XLUA_BLOCKSTATEMENT_H
#include <core/compiler/AST/StatementBase.h>
#include <core/Containers/Array.h>

namespace LXX {


class BlockStatement : public StatementBase
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    BlockStatement() = default;

    void AddStatement( StatementBase* statement );

    Array< StatementBase* >& GetStatements() { return _statements; }

private:
    Array< StatementBase* > _statements;
};


} // LXX

#endif //XLUA_BLOCKSTATEMENT_H
