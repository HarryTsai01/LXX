//
// Created by Harry on 9/11/2023.
//

#include "MetaMethodHandler.h"

namespace LXX
{


bool MetaMethodHandler::Invoke( State * state , const char * metaMethodKey , Value * destOperand , Value * srcOperand1 , Value * srcOperand2 )
{
    auto it = _metaMethodMap.Find( metaMethodKey );
    if( it != _metaMethodMap.End() )
    {
        (this->*(it->Second))( state , destOperand , srcOperand1 , srcOperand2 );
        return true;
    }

    return false;
}


} // LXX