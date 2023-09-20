//
// Created by Harry on 8/11/2023.
//

#include "StatementBase.h"


namespace LXX
{


StatementBase::StatementBase( u32 lineNo )
 :_lineNo( lineNo )
{

}


StatementBase::~StatementBase()
{
}



void StatementBase::CollectReferences( GCObjectCollector& collector )
{
}


} // LXX