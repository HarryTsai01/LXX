//
// Created by Harry on 9/11/2023.
//

#include "MetaMethodHandler.h"

namespace LXX
{


bool MetaMethodHandler::Invoke(const char * metaMethodKey , Value * destOperand , Value * srcOperand1 , Value * srcOperand2 )
{
    auto it = _binaryOperatorMap.Find( metaMethodKey );
    if( it != _binaryOperatorMap.End() )
    {
        (this->*(it->Second))( destOperand , srcOperand1 , srcOperand2 );
        return true;
    }

    return false;
}


} // LXX