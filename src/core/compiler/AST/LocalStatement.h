//
// Created by Harry on 8/11/2023.
//

#ifndef XLUA_LOCALSTATEMENT_H
#define XLUA_LOCALSTATEMENT_H
#include "StatementBase.h"

namespace LXX
{


class LocalStatement  : public StatementBase
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    using StatementBase::StatementBase;

    void SetChild( StatementBase* child ) { _child = child; }
    StatementBase* GetChild() { return _child; }

private:
    StatementBase* _child;
};


} // LXX

#endif //XLUA_LOCALSTATEMENT_H
