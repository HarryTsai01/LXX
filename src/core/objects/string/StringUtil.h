//
// Created by Harry on 8/9/2023.
//

#ifndef XLUA_STRINGUTIL_H
#define XLUA_STRINGUTIL_H
#include <core/LXX.h>
#include <core/objects/string/String.h>

namespace LXX
{


class StringUtil {

public:
    static u32 Hash( const char* str , u32 len );
    static String* CreateLongString( u32 len );

    static String* NewString( const char* begin , const char * end );

    static u32 StrLen( const char *str );


    static u32 Strncpy( char *dest , const char * src , u32 len );

    static String* Concat( String* str1 , String *str2 );
};


}



#endif //XLUA_STRINGUTIL_H
