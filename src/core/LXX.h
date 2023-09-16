//
// Created by Harry on 8/4/2023.
//

#ifndef XLUA_XLUA_H
#define XLUA_XLUA_H
#include <new>
#include <cstddef>
#include <core/LXX.config.h>


namespace LXX
{

#define ENABLE( x ) ( x == 1 )

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;
typedef float f32;
typedef double f64;


enum class ValueType : u8
{
    Start = 1 ,
    Number = Start ,
    LightUserdata = 2 ,
    Boolean = 3 ,
    String = 4 ,
    Nil = 5 ,
    Table = 6 ,
    Function = 7 ,
    Thread = 8 ,
    FullUserdata = 9,
    Max = FullUserdata ,
};


enum class NumberType : u8
{
    Integer,
    Real
};

enum class FunctionType : u8
{
    LuaClosure,
    LightCFunction,
    CClosure
};

enum class StringType : u8
{
    Long,
    Short
};

class State;

typedef u32 (*CFunction)(State*);

#define OPERATOR_NEW_DELETE_OVERRIDE_NORMAL \
public:                           \
    static void* operator new( size_t size ) \
    {\
        return MemoryAllocator::GetInstance().Allocate( size ); \
    }                         \
    static void operator delete( void* ptr ) \
    {                         \
        MemoryAllocator::GetInstance().Free( ptr );                         \
    }\
private:

#define OPERATOR_NEW_OVERLOAD_WITH_ADDITIONAL_SIZE \
public:                           \
    static void* operator new( size_t size , size_t additionalSize ) \
    {\
        return MemoryAllocator::GetInstance().Allocate( size + additionalSize ); \
    }                         \
    static void operator delete( void* ptr ) \
    {                         \
        MemoryAllocator::GetInstance().Free( ptr ); \
    }\
private:

#define OPERATOR_NEW_DELETE_OVERRIDE_ARRAY \
public:                           \
    static void* operator new[]( size_t size )\
    {                         \
        return MemoryAllocator::GetInstance().Allocate( size );                           \
    }                         \
    static void operator delete[]( void* ptr )                  \
    {                         \
        MemoryAllocator::GetInstance().Free( ptr );                          \
    }                         \
private:

#define OPERATOR_NEW_DELETE_OVERRIDE_ALL \
    OPERATOR_NEW_DELETE_OVERRIDE_NORMAL  \
    OPERATOR_NEW_DELETE_OVERRIDE_ARRAY


class IOStreamBase;

struct Version
{
    u32 _major;
    u32 _minor;
    u32 _build;
    u32 _revision;

    Version()
    : Version( 0 , 0 , 0 , 0 )
    {

    }

    Version( u32 major , u32 minor , u32 build , u32 revision )
        : _major( major )
        , _minor( minor )
        , _build( build )
        , _revision( revision )
    {

    }

    Version( const Version& other )
        : _major( other._major )
        , _minor( other._minor )
        , _build( other._build )
        , _revision( other._revision )
    {

    }

    bool operator == ( const Version &other ) const
    {
        return _major == other._major
            && _minor == other._minor
            && _build == other._build
            && _revision == other._revision;
    }

    bool operator != ( const Version &other ) const
    {
        return !( *this == other );
    }

    bool operator < ( const Version &other ) const
    {
        if( _major < other._major ) return true;
        if( _minor < other._minor ) return true;
        if( _build < other._build ) return true;
        if( _revision < other._revision ) return true;
        return false;
    }

    bool operator <= ( const Version &other ) const
    {
        return (*this < other) || (*this <= other);
    }

    bool operator >( const Version &other ) const
    {
        return other <= *this;
    }

    bool operator >=( const Version& other ) const
    {
        return  other > *this ;
    }

};
IOStreamBase& operator<<( IOStreamBase &stream , Version& version);
IOStreamBase& operator>>( IOStreamBase &stream , Version& version);

extern Version GLXXVersion;
extern Version GLXXDebuggerVersion;

} // namespace LXX

#endif //XLUA_XLUA_H
