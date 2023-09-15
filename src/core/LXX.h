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

} // namespace LXX

#endif //XLUA_XLUA_H
