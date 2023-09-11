//
// Created by Harry on 8/17/2023.
//

#ifndef XLUA_ASSIGNMENTSTATEMENT_H
#define XLUA_ASSIGNMENTSTATEMENT_H
#include "StatementBase.h"
#include <core/objects/GCObjectCollector.h>
#include <core/compiler/AST/Expressions/ExpressionStatementBase.h>

namespace LXX {


class AssignmentStatement :  public StatementBase
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    AssignmentStatement();
    virtual ~AssignmentStatement();

    void SetOperators( StatementBase* left, StatementBase* right );

    virtual void CollectReferences( GCObjectCollector& collector ) override;

    StatementBase* GetLeft() const {  return _left; }
    StatementBase* GetRight() const { return _right; }
private:
    StatementBase* _left;
    StatementBase* _right;

};



} // LXX

#endif //XLUA_ASSIGNMENTSTATEMENT_H
