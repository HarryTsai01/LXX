//
// Created by Harry on 8/11/2023.
//

#include "FunctionStatement.h"
#include <core/compiler/AST/Expressions/DotExpression.h>
#include <core/compiler/AST/Expressions/IdentifierExpression.h>

namespace LXX
{


FunctionStatement::FunctionStatement()
: _functionName( nullptr )
, _body( nullptr )
{

}


Array< String* > FunctionStatement::FetchParameterNames() const
{
    Array< String* > parameterNames;
    for(  auto parameter : _parameters )
    {
        if( auto identifer = parameter->Cast<IdentifierExpression>() )
        {
            parameterNames.PushBack( identifer->GetIdentifier() );
        }
    }
    return std::move( parameterNames );
}


FunctionStatement::~FunctionStatement()
{
}


void FunctionStatement::CollectReferences( GCObjectCollector &collector )
{
    StatementBase::CollectReferences( collector );

    collector.AddObject( _functionName );
    collector.AddObject( _body );
    collector.AddObjects( _parameters );
}


void FunctionStatement::SetFunctionName( StatementBase* functionName )
{
    _functionName = functionName;
}


void FunctionStatement::SetFunctionBody( StatementBase* body )
{
    _body = body;
}


void FunctionStatement::AddParameter( StatementBase* parameter )
{
    _parameters.PushBack( parameter );
}


void FunctionStatement::AddSelfParameter()
{
    _parameters.PushFront( new IdentifierExpression( NEW_STRING( "self" ) ) );
}

u32 FunctionStatement::GetParameterCount() const
{
    if (  !_parameters.IsEmpty() && _parameters.GetLast()->IsA<DotExpression>() )
    {
        return -1;
    }

    return _parameters.GetSize();
}


} // LXX