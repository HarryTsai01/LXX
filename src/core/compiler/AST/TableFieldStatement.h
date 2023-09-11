//
// Created by Harry on 8/30/2023.
//

#ifndef XLUA_TABLEFIELDSTATEMENT_H
#define XLUA_TABLEFIELDSTATEMENT_H
#include "StatementBase.h"

namespace LXX
{

/*
 *      field ::= ‘[’ exp ‘]’ ‘=’ exp | Name ‘=’ exp | exp
 * */
class TableFieldStatement : public StatementBase
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    TableFieldStatement();
    void SetKey( StatementBase* key ) { _key = key; }
    void SetValue( StatementBase* value ) { _value = value; }

    StatementBase* GetKey() { return _key; }
    StatementBase* GetValue() { return _value; }
private:
    StatementBase* _key;
    StatementBase* _value;
};

} // LXX

#endif //XLUA_TABLEFIELDSTATEMENT_H
