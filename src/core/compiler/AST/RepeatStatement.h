//
// Created by Harry on 8/11/2023.
//

#ifndef XLUA_REPEATSTATEMENT_H
#define XLUA_REPEATSTATEMENT_H
#include "LoopStatementBase.h"

namespace LXX {


class RepeatStatement : public LoopStatementBase
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    using LoopStatementBase::LoopStatementBase;
};


} // LXX

#endif //XLUA_REPEATSTATEMENT_H
