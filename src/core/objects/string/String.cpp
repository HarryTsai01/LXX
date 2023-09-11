//
// Created by Harry on 8/8/2023.
//

#include "String.h"
#include "StringTable.h"
#include <cstring>
#include <core/config/config.h>

namespace LXX
{

u32 Hash( const String* str )
{
    return Hash( str->GetData( ) );
}


u32 Hash( String* str )
{
    return Hash( (const String *)str );
}

template<>
bool IsEqual( String *lhs , String *rhs )
{
    return lhs == rhs || lhs->Compare( *rhs ) == 0;
}

std::ostream& operator<<( std::ostream &os , const String *str )
{
    os << str->GetData( );
    return os;
}

} // LXX