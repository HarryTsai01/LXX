//
// Created by Harry on 8/8/2023.
//

#ifndef XLUA_STRING_H
#define XLUA_STRING_H
#include <ostream>
#include <cstring>
#include <core/LXX.h>
#include <core/objects/GCObject.h>
#include <core/Containers/Array.h>
#include <initializer_list>
#include "StringTable.h"

namespace LXX
{


template<typename CharType = char >
class TString : public GCObject
{
public:
    OPERATOR_NEW_OVERLOAD_WITH_ADDITIONAL_SIZE
private:
    friend class StringTable;
    friend class StringUtil;
public:
    enum class Compare
    {
        CaseSensitive,
        CaseInsensitive,
    };

    u32 GetLength() const { return length; }
    const CharType *GetData() const { return data; }
    CharType *GetData() { return data; }

    bool IsEmpty() const { return length == 0; }
    bool IsEqual( const TString &other ) const
    {
        return Hash( data ) == Hash( other.data )
               && Compare( other.data , Compare::CaseSensitive ) == 0;
    }
    bool IsEqual( const CharType *str ) const
    {
        return Hash( data ) == Hash( str )
               && Compare( str , Compare::CaseSensitive ) == 0;
    }

    s32 Compare( const TString &other , enum  Compare mode = Compare::CaseSensitive ) const
    {
        return Compare( other.data , mode );
    }

    u32 Compare( const CharType *str  ,  enum Compare mode = Compare::CaseSensitive ) const
    {
        auto convertChar = []( CharType ch ,  enum Compare mode ) -> s32
        {
            bool isUpper = ( ch >= 'A' && ch <= 'Z' );
            return mode == Compare::CaseInsensitive && isUpper  ? ch - 'A' + 'a' : ch;
        };
        u32 destLen = strlen( str );
        u32 len = length < destLen ? length : destLen;
        for( u32 i = 0 ; i < len ; i++ )
        {
            u32 delta = convertChar( data[i] , mode ) - convertChar( str[i] , mode );
            if( delta != 0 )
                return delta < 0 ? -1 : 1;
        }

        return length < destLen ? -1 : length > destLen ? 1 : 0;
    }

    bool operator==( const TString &other ) const
    {
        return IsEqual( other );
    }


    bool operator!=( const TString &other ) const
    {
        return !(*this == other);
    }

    bool operator==( const CharType *str ) const
    {
        return IsEqual( str );
    }


    bool operator!=( const CharType *str ) const
    {
        return !(*this == str);
    }

    StringType GetType() const
    {
        return length <= Config::MAX_SHORT_STRING_LENGTH ? StringType::Short : StringType::Long;
    }

    template<typename ... Args>
    static TString<>* Format( const char *format , Args ... args )
    {
        u32 size = snprintf(nullptr, 0, format, args...) ;
        TString<> *str = StringTable::GetInstance().CreateString( size );
        snprintf( str->data, size + 1, format, std::forward<Args>(args)...);
        return str;
    }

private:
    TString( const CharType *str , u32 len )
    {
        memcpy( data , str , len );
        data[len] = '\0';
        length = len;
    }
    TString(  u32 len )
    {
        memset(  data , 0 , len );
        data[len] = '\0';
        length = len;
    }
    TString( const String &other ) = delete;
    TString( String &&other ) = delete;
    TString &operator=( const String &other ) = delete;
    TString &operator=( String &&other ) = delete;
private:
    u32 length;
    CharType data[1];
};

typedef TString<char> String;


template<typename T>
String* ToString( T value );
u32 Hash( const String* str );
u32 Hash( String* str );

template<>
bool IsEqual( String *lhs , String *rhs );

std::ostream& operator<<( std::ostream &os , const String *str );


#define NEW_STRING( str ) StringTable::GetInstance().FindOrCreateString( str )
#define NEW_STRING_WITH_SIZE( len ) StringTable::GetInstance().CreateString( len )

} // LXX

#endif //XLUA_STRING_H
