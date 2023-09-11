//
// Created by Harry on 8/30/2023.
//

#ifndef XLUA_TABLECONSTRUCTORSTATEMENT_H
#define XLUA_TABLECONSTRUCTORSTATEMENT_H
#include "StatementBase.h"
#include <core/Containers/Array.h>

namespace LXX
{


class TableConstructorStatement : public StatementBase
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    TableConstructorStatement() = default;

    void AddField( StatementBase *field );

    Array< StatementBase * >& GetFields() { return _fields; }

private:
    Array< StatementBase * > _fields;
};


} // LXX

#endif //XLUA_TABLECONSTRUCTORSTATEMENT_H
