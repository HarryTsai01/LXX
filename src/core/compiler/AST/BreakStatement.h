//
// Created by Harry on 8/29/2023.
//

#ifndef XLUA_BREAKSTATEMENT_H
#define XLUA_BREAKSTATEMENT_H
#include "StatementBase.h"

namespace LXX {


class BreakStatement :  public StatementBase
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    using StatementBase::StatementBase;
};


} // LXX

#endif //XLUA_BREAKSTATEMENT_H
