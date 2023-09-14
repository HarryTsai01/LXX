//
// Created by Harry on 9/11/2023.
//

#include "MetaMethodHandlerBoolean.h"

namespace LXX
{


MetaMethodHandlerBoolean::MetaMethodHandlerBoolean()
{
    META_METHOD_BEGIN_REGISTER()
        META_METHOD_REGISTER( MetaMethodHandlerBoolean,META_METHOD_KEY_BIN_OP_EQUAL )
    META_METHOD_END_REGISTER()
}


IMPLEMENT_META_METHOD( MetaMethodHandlerBoolean, META_METHOD_KEY_BIN_OP_EQUAL )
{
    if( destOperand == nullptr )
        ThrowError( "destOperand is null in MetaMethodHandlerNumber::META_METHOD_KEY_BIN_OP_EQUAL" );
    if( !srcOperand1->IsBoolean() )
        ThrowError( "srcOperand1 is not boolean in MetaMethodHandlerNumber::META_METHOD_KEY_BIN_OP_EQUAL" );
    if( !srcOperand2->IsBoolean() )
        ThrowError( "srcOperand2 is not boolean in MetaMethodHandlerNumber::META_METHOD_KEY_BIN_OP_EQUAL" );

    bool result = srcOperand1->As<bool>() == srcOperand2->As<bool>();
    destOperand->SetAsBoolean( result );
}


} // LXX