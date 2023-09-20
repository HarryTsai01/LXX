//
// Created by Harry on 8/30/2023.
//

#ifndef XLUA_FUNCTIONNAMESTATEMENT_H
#define XLUA_FUNCTIONNAMESTATEMENT_H
#include "StatementBase.h"
#include <core/Containers/Array.h>

namespace LXX {


class FunctionNameStatement : public StatementBase
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    FunctionNameStatement( u32 lineNo ) ;

    void SetLastConcatToken( s32 lastConcatToken );
    void AddName( StatementBase* name );

    Array< StatementBase* >& GetNames() { return _names; }
    s32 GetLastConcatToken() { return _lastConcatToken; }
private:
    s32 _lastConcatToken;
    Array< StatementBase* > _names;
};


} // LXX

#endif //XLUA_FUNCTIONNAMESTATEMENT_H
