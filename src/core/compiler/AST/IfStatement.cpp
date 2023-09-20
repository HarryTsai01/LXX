//
// Created by Harry on 8/11/2023.
//

#include "IfStatement.h"

namespace LXX
{


IfStatement::IfStatement( u32 lineNo )
    : StatementBase( lineNo )
    , _condition( nullptr )
    , _then ( nullptr )
    , _else ( nullptr )

{

}


} // LXX