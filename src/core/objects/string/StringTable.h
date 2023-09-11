//
// Created by Harry on 8/9/2023.
//

#ifndef XLUA_STRINGTABLE_H
#define XLUA_STRINGTABLE_H
#include <core/Containers/UnorderedMap.h>
#include <core/config/Config.h>
#include <core/Containers/DisableEnhancePolicy.h>
#include <core/Containers/HashBucket.h>

namespace LXX
{

template< typename CharType >
class TString;
typedef TString< char > String;

class StringTable
{
public:
    StringTable();
    ~StringTable();

    static StringTable& GetInstance();

    String* FindOrCreateString( const char* str );
    String* CreateString( u32 len );

private:
    typedef HashBucket< const char * , String* , Config::STRING_TABLE_MIN_SLOT_SIZE> StringBucket;
    StringBucket stringBucket;

    static StringTable* s_Instance;
};

} // LXX

#endif //XLUA_STRINGTABLE_H
