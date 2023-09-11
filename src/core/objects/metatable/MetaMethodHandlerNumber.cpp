//
// Created by Harry on 9/11/2023.
//

#include "MetaMethodHandlerNumber.h"
#include <cmath>

namespace LXX
{


MetaMethodHandlerNumber::MetaMethodHandlerNumber()
{
    META_METHOD_BEGIN_REGISTER()
        META_METHOD_REGISTER( MetaMethodHandlerNumber, META_METHOD_KEY_BIN_OP_ADD )
        META_METHOD_REGISTER( MetaMethodHandlerNumber, META_METHOD_KEY_BIN_OP_SUB )
        META_METHOD_REGISTER( MetaMethodHandlerNumber,META_METHOD_KEY_BIN_OP_MUL )
        META_METHOD_REGISTER( MetaMethodHandlerNumber,META_METHOD_KEY_BIN_OP_DIV )
        META_METHOD_REGISTER( MetaMethodHandlerNumber,META_METHOD_KEY_BIN_OP_MOD )
        META_METHOD_REGISTER( MetaMethodHandlerNumber,META_METHOD_KEY_BIN_OP_POW )
        META_METHOD_REGISTER( MetaMethodHandlerNumber,META_METHOD_KEY_UNA_OP_MINUS )
        META_METHOD_REGISTER( MetaMethodHandlerNumber,META_METHOD_KEY_BIN_OP_IDIV )
        META_METHOD_REGISTER( MetaMethodHandlerNumber,META_METHOD_KEY_BIN_OP_BITWISE_AND )
        META_METHOD_REGISTER( MetaMethodHandlerNumber,META_METHOD_KEY_BIN_OP_BITWISE_OR )
        META_METHOD_REGISTER( MetaMethodHandlerNumber,META_METHOD_KEY_BIN_OP_BITWISE_XOR )
        META_METHOD_REGISTER( MetaMethodHandlerNumber,META_METHOD_KEY_UNA_OP_BITWISE_NOT )
        META_METHOD_REGISTER( MetaMethodHandlerNumber,META_METHOD_KEY_BIN_OP_SHIFT_LEFT )
        META_METHOD_REGISTER( MetaMethodHandlerNumber,META_METHOD_KEY_BIN_OP_SHIFT_RIGHT )
        META_METHOD_REGISTER( MetaMethodHandlerNumber,META_METHOD_KEY_BIN_OP_CONCAT )
        META_METHOD_REGISTER( MetaMethodHandlerNumber,META_METHOD_KEY_BIN_OP_EQUAL )
        META_METHOD_REGISTER( MetaMethodHandlerNumber,META_METHOD_KEY_BIN_OP_LESS_THAN )
        META_METHOD_REGISTER( MetaMethodHandlerNumber,META_METHOD_KEY_BIN_OP_LESS_THAN_OR_EQUAL )
    META_METHOD_END_REGISTER()
}

IMPLEMENT_META_METHOD( MetaMethodHandlerNumber, META_METHOD_KEY_BIN_OP_ADD )
{
    if( destOperand == nullptr )
       ThrowError( "destOperand is null in MetaMethodHandlerNumber::META_METHOD_KEY_BIN_OP_ADD" );

    f64 src1 = srcOperand1->AsReal();
    f64 src2 = srcOperand2->AsReal();
    f64 result = src1 + src2;

    if( srcOperand1->IsReal() || srcOperand2->IsReal() )
    {
        destOperand->SetAsReal( result );
    }
    else
    {
       destOperand->SetAsInt( result );
    }
}


IMPLEMENT_META_METHOD( MetaMethodHandlerNumber,META_METHOD_KEY_BIN_OP_SUB )
{
    if( destOperand == nullptr )
        ThrowError( "destOperand is null in MetaMethodHandlerNumber::META_METHOD_KEY_BIN_OP_SUB" );

    f64 src1 = srcOperand1->AsReal();
    f64 src2 = srcOperand2->AsReal();
    f64 result = src1 - src2;

    if( srcOperand1->IsReal() || srcOperand2->IsReal() )
    {
        destOperand->SetAsReal( result );
    }
    else
    {
        destOperand->SetAsInt( result );
    }
}


IMPLEMENT_META_METHOD( MetaMethodHandlerNumber,META_METHOD_KEY_BIN_OP_MUL )
{
    if( destOperand == nullptr )
        ThrowError( "destOperand is null in MetaMethodHandlerNumber::META_METHOD_KEY_BIN_OP_MUL" );

    f64 src1 = srcOperand1->AsReal();
    f64 src2 = srcOperand2->AsReal();
    f64 result = src1 * src2;

    if( srcOperand1->IsReal() || srcOperand2->IsReal() )
    {
        destOperand->SetAsReal( result );
    }
    else
    {
        destOperand->SetAsInt( result );
    }
}


IMPLEMENT_META_METHOD( MetaMethodHandlerNumber,META_METHOD_KEY_BIN_OP_DIV )
{
    if( destOperand == nullptr )
        ThrowError( "destOperand is null in MetaMethodHandlerNumber::META_METHOD_KEY_BIN_OP_DIV" );

    f64 src1 = srcOperand1->AsReal();
    f64 src2 = srcOperand2->AsReal();

    if( std::fabs( src2 ) < 0.000001 )
        ThrowError( "Division by zero in MetaMethodHandlerNumber::META_METHOD_KEY_BIN_OP_DIV" );

    f64 result = src1 / src2;

    if( srcOperand1->IsReal() || srcOperand2->IsReal() )
    {
        destOperand->SetAsReal( result );
    }
    else
    {
        destOperand->SetAsInt( result );
    }
}


IMPLEMENT_META_METHOD( MetaMethodHandlerNumber,META_METHOD_KEY_BIN_OP_MOD )
{
    if( destOperand == nullptr )
        ThrowError( "destOperand is null in MetaMethodHandlerNumber::META_METHOD_KEY_BIN_OP_MOD" );

    f64 src1 = srcOperand1->AsReal();
    f64 src2 = srcOperand2->AsReal();
    if( std::fabs( src2 ) < 0.000001 )
        ThrowError( "Division by zero in MetaMethodHandlerNumber::META_METHOD_KEY_BIN_OP_MOD" );

    f64 result = std::fmod( src1, src2 );

    if( srcOperand1->IsReal() || srcOperand2->IsReal() )
    {
        destOperand->SetAsReal( result );
    }
    else
    {
        destOperand->SetAsInt( result );
    }
}


IMPLEMENT_META_METHOD( MetaMethodHandlerNumber,META_METHOD_KEY_BIN_OP_POW )
{
    if( destOperand == nullptr )
        ThrowError( "destOperand is null in MetaMethodHandlerNumber::META_METHOD_KEY_BIN_OP_POW" );

    f64 src1 = srcOperand1->AsReal();
    f64 src2 = srcOperand2->AsReal();
    f64 result = std::pow( src1, src2 );

    if( srcOperand1->IsReal() || srcOperand2->IsReal() )
    {
        destOperand->SetAsReal( result );
    }
    else
    {
        destOperand->SetAsInt( result );
    }
}


IMPLEMENT_META_METHOD( MetaMethodHandlerNumber,META_METHOD_KEY_UNA_OP_MINUS )
{
    if( destOperand == nullptr )
        ThrowError( "destOperand is null in MetaMethodHandlerNumber::META_METHOD_KEY_UNA_OP_MINUS" );

    if( srcOperand1->IsReal() )
    {
        destOperand->SetAsReal( -srcOperand1->AsReal() );
    }
    else
    {
        destOperand->SetAsInt( -srcOperand1->AsInt() );
    }
}


IMPLEMENT_META_METHOD( MetaMethodHandlerNumber,META_METHOD_KEY_BIN_OP_IDIV )
{
    if( destOperand == nullptr )
        ThrowError( "destOperand is null in MetaMethodHandlerNumber::META_METHOD_KEY_BIN_OP_IDIV" );

    s32 src1 = srcOperand1->AsInt();
    s32 src2 = srcOperand2->AsInt();
    if( src2 == 0 )
        ThrowError( "Division by zero in MetaMethodHandlerNumber::META_METHOD_KEY_BIN_OP_IDIV" );
    s32 result = src1 / src2;

    destOperand->SetAsInt( result );
}


IMPLEMENT_META_METHOD( MetaMethodHandlerNumber,META_METHOD_KEY_BIN_OP_BITWISE_AND )
{
    if ( destOperand == nullptr )
        ThrowError( "destOperand is null in MetaMethodHandlerNumber::META_METHOD_KEY_BIN_OP_BITWISE_AND" );
    if( srcOperand1->IsReal() || srcOperand2->IsReal() )
        ThrowError( "Bitwise AND is not supported for real numbers" );
    s32 src1 = srcOperand1->AsInt();
    s32 src2 = srcOperand2->AsInt();
    s32 result = src1 & src2;
    destOperand->SetAsInt( result );
}


IMPLEMENT_META_METHOD( MetaMethodHandlerNumber,META_METHOD_KEY_BIN_OP_BITWISE_OR )
{
    if ( destOperand == nullptr )
        ThrowError( "destOperand is null in MetaMethodHandlerNumber::META_METHOD_KEY_BIN_OP_BITWISE_OR" );
    if( srcOperand1->IsReal() || srcOperand2->IsReal() )
        ThrowError( "Bitwise OR is not supported for real numbers" );
    s32 src1 = srcOperand1->AsInt();
    s32 src2 = srcOperand2->AsInt();
    s32 result = src1 | src2;
    destOperand->SetAsInt( result );
}


IMPLEMENT_META_METHOD( MetaMethodHandlerNumber,META_METHOD_KEY_BIN_OP_BITWISE_XOR )
{
    if ( destOperand == nullptr )
        ThrowError( "destOperand is null in MetaMethodHandlerNumber::META_METHOD_KEY_BIN_OP_BITWISE_XOR" );
    if( srcOperand1->IsReal() || srcOperand2->IsReal() )
        ThrowError( "Bitwise XOR is not supported for real numbers" );
    s32 src1 = srcOperand1->AsInt();
    s32 src2 = srcOperand2->AsInt();
    s32 result = src1 ^ src2;
    destOperand->SetAsInt( result );
}


IMPLEMENT_META_METHOD( MetaMethodHandlerNumber,META_METHOD_KEY_UNA_OP_BITWISE_NOT )
{
    if ( destOperand == nullptr )
        ThrowError( "destOperand is null in MetaMethodHandlerNumber::META_METHOD_KEY_UNA_OP_BITWISE_NOT" );
    if( srcOperand1->IsReal() )
        ThrowError( "Bitwise NOT is not supported for real numbers" );
    s32 src1 = srcOperand1->AsInt();
    s32 result = ~src1;
    destOperand->SetAsInt( result );
}


IMPLEMENT_META_METHOD( MetaMethodHandlerNumber,META_METHOD_KEY_BIN_OP_SHIFT_LEFT )
{
    if ( destOperand == nullptr )
        ThrowError( "destOperand is null in MetaMethodHandlerNumber::META_METHOD_KEY_BIN_OP_SHIFT_LEFT" );
    if( srcOperand1->IsReal() || srcOperand2->IsReal() )
        ThrowError( "Bitwise shift is not supported for real numbers" );
    s32 src1 = srcOperand1->AsInt();
    s32 src2 = srcOperand2->AsInt();
    s32 result = src1 << src2;
    destOperand->SetAsInt( result );
}


IMPLEMENT_META_METHOD( MetaMethodHandlerNumber,META_METHOD_KEY_BIN_OP_SHIFT_RIGHT )
{
    if( destOperand == nullptr )
        ThrowError( "destOperand is null in MetaMethodHandlerNumber::META_METHOD_KEY_BIN_OP_SHIFT_RIGHT" );
    if( srcOperand1->IsReal() || srcOperand2->IsReal() )
        ThrowError( "Bitwise shift is not supported for real numbers" );
    s32 src1 = srcOperand1->AsInt();
    s32 src2 = srcOperand2->AsInt();
    s32 result = src1 >> src2;
    destOperand->SetAsInt( result );
}


IMPLEMENT_META_METHOD( MetaMethodHandlerNumber,META_METHOD_KEY_BIN_OP_CONCAT )
{
    if( destOperand == nullptr )
        ThrowError( "destOperand is null in MetaMethodHandlerNumber::META_METHOD_KEY_BIN_OP_CONCAT" );
    String* result = nullptr;
    if( srcOperand1->IsReal() )
    {
        if(  srcOperand2->IsReal() )
            result = String::Format("%f%f", srcOperand1->AsReal(), srcOperand2->AsReal());
        else
            result = String::Format("%f%d", srcOperand1->AsReal(), srcOperand2->AsInt());
    }
    else
    {
        if( srcOperand2->IsReal() )
            result = String::Format("%d%f", srcOperand1->AsInt(), srcOperand2->AsReal());
        else
            result = String::Format("%d%d", srcOperand1->AsInt(), srcOperand2->AsInt());
    }

    destOperand->SetAsString( result );
}


IMPLEMENT_META_METHOD( MetaMethodHandlerNumber,META_METHOD_KEY_BIN_OP_EQUAL )
{
    if( destOperand == nullptr )
        ThrowError( "destOperand is null in MetaMethodHandlerNumber::META_METHOD_KEY_BIN_OP_EQUAL" );

    f64 src1 = srcOperand1->AsReal();
    f64 src2 = srcOperand2->AsReal();
    bool result = src1 == src2;
    destOperand->SetAsBoolean( result );
}


IMPLEMENT_META_METHOD( MetaMethodHandlerNumber,META_METHOD_KEY_BIN_OP_LESS_THAN )
{
    if( destOperand == nullptr )
        ThrowError( "destOperand is null in MetaMethodHandlerNumber::META_METHOD_KEY_BIN_OP_LESS_THAN" );

    f64 src1 = srcOperand1->AsReal();
    f64 src2 = srcOperand2->AsReal();
    bool result = src1 < src2;
    destOperand->SetAsBoolean( result );
}


IMPLEMENT_META_METHOD( MetaMethodHandlerNumber,META_METHOD_KEY_BIN_OP_LESS_THAN_OR_EQUAL )
{
    if( destOperand == nullptr )
        ThrowError( "destOperand is null in MetaMethodHandlerNumber::META_METHOD_KEY_BIN_OP_LESS_THAN_OR_EQUAL" );

    f64 src1 = srcOperand1->AsReal();
    f64 src2 = srcOperand2->AsReal();
    bool result = src1 <= src2;
    destOperand->SetAsBoolean( result );
}

} // LXX