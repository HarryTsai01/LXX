//
// Created by Harry on 8/11/2023.
//

#ifndef XLUA_IFSTATEMENT_H
#define XLUA_IFSTATEMENT_H
#include "StatementBase.h"
#include <core/Containers/Array.h>

namespace LXX {

class IfStatement : public StatementBase
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    IfStatement() ;

    StatementBase* GetCondition() const { return _condition; }
    StatementBase* GetThen() const { return _then; }
    StatementBase* GetElse() const { return _else; }
    void GetElseif(Array< StatementBase* >& elseif) const { elseif = _elseif; }

    void SetCondition( StatementBase* condition ) { _condition = condition; }
    void SetThen( StatementBase* then ) { _then = then; }
    void SetElse( StatementBase* _else ) { this->_else = _else; }
    void AddElseif( StatementBase* elseif ) { _elseif.Add(elseif); }

private:
    StatementBase* _condition;
    StatementBase* _then;
    Array< StatementBase* > _elseif;
    StatementBase* _else;
};

} // LXX

#endif //XLUA_IFSTATEMENT_H
