//
// Created by Harry on 8/30/2023.
//

#include "TableConstructorStatement.h"

namespace LXX
{


void TableConstructorStatement::AddField( StatementBase *field )
{
    _fields.PushBack( field );
}


} // xLua