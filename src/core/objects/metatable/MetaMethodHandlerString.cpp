//
// Created by Harry on 9/11/2023.
//

#include "MetaMethodHandlerString.h"

namespace LXX
{


MetaMethodHandlerString::MetaMethodHandlerString()
{
    META_METHOD_BEGIN_REGISTER()
        META_METHOD_REGISTER( MetaMethodHandlerString,META_METHOD_KEY_INDEX )
        META_METHOD_REGISTER( MetaMethodHandlerString,META_METHOD_KEY_NEW_INDEX )
        META_METHOD_REGISTER( MetaMethodHandlerString,META_METHOD_KEY_UNA_OP_LEN )
        META_METHOD_REGISTER( MetaMethodHandlerString,META_METHOD_KEY_BIN_OP_ADD )
        META_METHOD_REGISTER( MetaMethodHandlerString,META_METHOD_KEY_BIN_OP_CONCAT )
        META_METHOD_REGISTER( MetaMethodHandlerString,META_METHOD_KEY_BIN_OP_EQUAL )
        META_METHOD_REGISTER( MetaMethodHandlerString,META_METHOD_KEY_BIN_OP_LESS_THAN )
        META_METHOD_REGISTER( MetaMethodHandlerString,META_METHOD_KEY_BIN_OP_LESS_THAN_OR_EQUAL )
    META_METHOD_END_REGISTER()
}


IMPLEMENT_META_METHOD( MetaMethodHandlerString, META_METHOD_KEY_INDEX )
{
    if( destOperand == nullptr )
        ThrowError( "destOperand is null in MetaMethodHandlerString::META_METHOD_KEY_INDEX" );
    if( !srcOperand2->IsInteger() )
        ThrowError( "try to get index of non-integer in string" );
    String* src = srcOperand1->ToString();
    u32 len = src->GetLength();
    s32 index = srcOperand2->As<s32>();
    if( index < 0 )
        index += len;
    if( index < 0 || index >= len )
        ThrowError( "index out of range , index:%d" , index );

    char c = src->GetData()[index];

    destOperand->Set( c );
}


IMPLEMENT_META_METHOD( MetaMethodHandlerString, META_METHOD_KEY_NEW_INDEX )
{
    ThrowError( "string can not be changed." );
}


IMPLEMENT_META_METHOD( MetaMethodHandlerString, META_METHOD_KEY_UNA_OP_LEN )
{
    if( destOperand == nullptr )
        ThrowError( "destOperand is null in MetaMethodHandlerNumber::META_METHOD_KEY_UNA_OP_LEN" );

    String* src = srcOperand1->ToString();
    s32 result = src->GetLength();

    destOperand->Set( result );
}


IMPLEMENT_META_METHOD( MetaMethodHandlerString, META_METHOD_KEY_BIN_OP_ADD )
{
    if( destOperand == nullptr )
        ThrowError( "destOperand is null in MetaMethodHandlerNumber::META_METHOD_KEY_BIN_OP_ADD" );

    String* src1 = srcOperand1->ToString();
    String* src2 = srcOperand2->ToString();
    String* result = String::Format( "%s%s", src1->GetData(), src2->GetData() );

    destOperand->SetAsString( result );
}


IMPLEMENT_META_METHOD( MetaMethodHandlerString, META_METHOD_KEY_BIN_OP_CONCAT )
{
    if( destOperand == nullptr )
        ThrowError( "destOperand is null in MetaMethodHandlerNumber::META_METHOD_KEY_BIN_OP_CONCAT" );

    String* src1 = srcOperand1->ToString();
    String* src2 = srcOperand2->ToString();
    String* result = String::Format( "%s%s", src1->GetData(), src2->GetData() );

    destOperand->SetAsString( result );
}


IMPLEMENT_META_METHOD( MetaMethodHandlerString, META_METHOD_KEY_BIN_OP_EQUAL )
{
    if( destOperand == nullptr )
        ThrowError( "destOperand is null in MetaMethodHandlerNumber::META_METHOD_KEY_BIN_OP_EQUAL" );

    String* src1 = srcOperand1->ToString();
    String* src2 = srcOperand2->ToString();
    bool result = src1->Compare( *src2 ) == 0;

    destOperand->SetAsBoolean( result );
}


IMPLEMENT_META_METHOD( MetaMethodHandlerString, META_METHOD_KEY_BIN_OP_LESS_THAN )
{
    if( destOperand == nullptr )
        ThrowError( "destOperand is null in MetaMethodHandlerNumber::META_METHOD_KEY_BIN_OP_LESS_THAN" );
    String* src1 = srcOperand1->ToString();
    String* src2 = srcOperand2->ToString();
    bool result = src1->Compare( *src2 ) == -1;

    destOperand->SetAsBoolean( result );
}


IMPLEMENT_META_METHOD( MetaMethodHandlerString, META_METHOD_KEY_BIN_OP_LESS_THAN_OR_EQUAL )
{
    if( destOperand == nullptr )
        ThrowError( "destOperand is null in MetaMethodHandlerNumber::META_METHOD_KEY_BIN_OP_LESS_THAN_OR_EQUAL" );
    String* src1 = srcOperand1->ToString();
    String* src2 = srcOperand2->ToString();
    s32 compareResult = src1->Compare( *src2 ) ;
    bool result = compareResult == -1 || compareResult == 0;

    destOperand->SetAsBoolean( result );
}

} // LXX