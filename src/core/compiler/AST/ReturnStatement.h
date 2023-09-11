//
// Created by Harry on 8/11/2023.
//

#ifndef XLUA_RETURNSTATEMENT_H
#define XLUA_RETURNSTATEMENT_H
#include "StatementBase.h"

namespace LXX
{


class ReturnStatement : public StatementBase
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    ReturnStatement();
    void SetReturnValue( StatementBase* returnValue ) { _returnValue = returnValue; }
    StatementBase* GetReturnValue() { return _returnValue; }
private:
    StatementBase* _returnValue;
};


} // LXX

#endif //XLUA_RETURNSTATEMENT_H
