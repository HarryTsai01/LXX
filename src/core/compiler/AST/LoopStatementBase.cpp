//
// Created by Harry on 8/11/2023.
//

#include "LoopStatementBase.h"

namespace LXX
{


LoopStatementBase::LoopStatementBase( u32 lineNo )
    : StatementBase( lineNo )
    , _condition( nullptr )
    , _body( nullptr )
{

}


} // LXX