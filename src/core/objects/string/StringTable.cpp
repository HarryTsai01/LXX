//
// Created by Harry on 8/9/2023.
//

#include "StringTable.h"
#include "String.h"
#include "StringUtil.h"
#include <core/mem/MemoryAllocator.h>
#include <cstring>

namespace LXX {


StringTable* StringTable::s_Instance = nullptr;

bool IsEqual( String* lhs , const char* rhs )
{
    if(  lhs == nullptr ) return false;
    return lhs->IsEqual( rhs );
}

StringTable::StringTable()
{
}

StringTable& StringTable::GetInstance()
{
    if( s_Instance == nullptr )
    {
        s_Instance = new StringTable();
    }

    return *s_Instance;
}

String* StringTable::FindOrCreateString( const char *str )
{
    if( str == nullptr ) return nullptr;
    u32 len = strlen( str );
    if( len == 0 ) return nullptr;

    if ( len <= Config::MAX_SHORT_STRING_LENGTH )
    {
        String* foundStr = nullptr;
        if( stringBucket.TryGet( str , foundStr ) )
        {
            return foundStr;
        }
        else
        {
            String* newString = new (len) String(str , len);
            stringBucket.Add( str , newString );
            return newString;
        }
    }

    // create new string
    return new ( len ) String(str , len );
}


String* StringTable::CreateString(u32 len)
{
    return new( len ) String( len );
}


} // LXX