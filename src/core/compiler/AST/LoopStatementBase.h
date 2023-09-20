//
// Created by Harry on 8/11/2023.
//

#ifndef XLUA_LOOPSTATEMENTBASE_H
#define XLUA_LOOPSTATEMENTBASE_H
#include "StatementBase.h"

namespace LXX
{

class LoopStatementBase : public StatementBase
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    LoopStatementBase( u32 lineNo );

    void SetCondition( StatementBase *condition ) { _condition = condition; }
    void SetBody( StatementBase *body ) { _body = body; }

    StatementBase *GetCondition() { return _condition; }
    StatementBase *GetBody() { return _body; }

protected:
    StatementBase *_condition;
    StatementBase *_body;
};

} // LXX

#endif //XLUA_LOOPSTATEMENTBASE_H
