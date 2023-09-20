//
// Created by Harry on 8/31/2023.
//

#ifndef XLUA_CONTINUESTATEMENT_H
#define XLUA_CONTINUESTATEMENT_H
#include "StatementBase.h"

namespace LXX {

class ContinueStatement : public StatementBase
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    using StatementBase::StatementBase;
};

} // LXX

#endif //XLUA_CONTINUESTATEMENT_H
