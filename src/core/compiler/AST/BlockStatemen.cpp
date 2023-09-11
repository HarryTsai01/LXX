//
// Created by Harry on 8/28/2023.
//

#include "BlockStatement.h"

namespace LXX
{


void BlockStatement::AddStatement( StatementBase *statement )
{
    _statements.PushBack( statement );
}


} // xLua