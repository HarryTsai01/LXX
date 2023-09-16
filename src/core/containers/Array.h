//
// Created by Harry on 8/24/2023.
//

#ifndef XLUA_ARRAY_H
#define XLUA_ARRAY_H
#include <cassert>
#include <cstring>
#include <initializer_list>
#include <iterator>
#include <core/mem/MemoryAllocator.h>
#include <core/Containers/DefaultEnhancePolicy.h>
#include "Iterator.h"


namespace LXX
{


template<typename ElementType , typename EnhancePolicy =  DefaultEnhancePolicy, typename Allocator = MemoryAllocator >
class Array
{
    friend class Iterator< Array >;
    friend class ReverseIterator< Array >;
    friend class ConstIterator< Array >;
public:
    typedef u32 IndexType;
    typedef ElementType ValueType;
    typedef s32 DifferenceType;

    typedef Iterator< Array > IteratorType;
    typedef ReverseIterator< Array > ReverseIteratorType;
    typedef ConstIterator< Array > ConstIteratorType;

    bool IsValidIndex( u32 index ) const
    {
        return index < m_Size;
    }
    void OffsetIndex( u32& index , s32 offset )
    {
        index += offset;
    }

    IteratorType Begin() { return IteratorType( this , 0 ); }
    IteratorType End() { return IteratorType( this , m_Size ); }
    ConstIteratorType ConstBegin() const { return ConstIteratorType ( this , 0 ); }
    ConstIteratorType ConstEnd() const { return ConstIteratorType ( this , m_Size ); }
    ReverseIteratorType ReverseBegin() { return ReverseIteratorType ( this , m_Size ); }
    ReverseIteratorType  ReverseEnd() { return ReverseIteratorType ( this , 0 ); }

    IteratorType begin() { return Begin(); }
    IteratorType end() { return End(); }
    ConstIteratorType begin() const { return ConstBegin(); }
    ConstIteratorType end() const { return ConstEnd(); }
    ReverseIteratorType  rbegin() { return ReverseBegin(); }
    ReverseIteratorType  rend() { return ReverseEnd(); }

    Array( u32 initialSize = 0 )
    {
        m_pAllocator = &Allocator::GetInstance();
        m_Capacity = EnhancePolicy::GetInitialCapacity();
        m_Size = 0;
        m_pData = nullptr;

        if( initialSize > 0 )
        {
            AddNum( initialSize );
        }
        else
        {
            m_pData = static_cast<ElementType*>( m_pAllocator->Allocate( m_Capacity * sizeof(ElementType) ) );
        }
    }


    Array( const Array& other )
    {
        m_pAllocator = &Allocator::GetInstance();
        m_Size = other.Size();
        m_Capacity = other.GetCapacity();
        m_pData = static_cast<ElementType*>( m_pAllocator->Allocate( m_Capacity * sizeof(ElementType) ) );
        memcpy( m_pData , other.m_pData , m_Size * sizeof(ElementType) );
    }

    Array( const std::initializer_list<ValueType>& _list )
    {
        m_pAllocator = &Allocator::GetInstance();
        m_Size = _list.size();
        m_Capacity = EnhancePolicy::CalculateProperCapacity( m_Size );
        m_pData = static_cast<ElementType*>( m_pAllocator->Allocate( m_Capacity * sizeof(ElementType) ) );
        u32 i = 0;
        for( const auto & element : _list )
        {
            m_pData[i] = element;
            ++i;
        }
    }

    template<typename OtherElementType>
    Array( const Array<OtherElementType>& other )
    {
        m_pAllocator = &Allocator::GetInstance();
        m_Size = other.GetSize();
        m_Capacity = other.GetCapacity();
        m_pData = static_cast<ElementType*>( m_pAllocator->Allocate( m_Capacity * sizeof(ElementType) ) );
        for( u32 i = 0 ; i < m_Size ; ++i )
        {
            m_pData[ i ] = other[ i ];
        }
    }

    Array( Array&& other )
    {
        m_pAllocator = other.m_pAllocator;
        m_Size = other.m_Size;
        m_Capacity = other.m_Capacity;
        m_pData = other.m_pData;

        other.m_pAllocator = nullptr;
        other.m_Size = 0;
        other.m_Capacity = 0;
        other.m_pData = nullptr;
    }

    ~Array()
    {
        Dispose();
    }


    ElementType* Add()
    {
        return Add( ElementType() );
    }


    ElementType* Add( const ElementType& element )
    {
        ElementType* newElement = AddUninitialized();
        *newElement = element;
        return newElement;
    }


    ElementType* AddUninitialized()
    {
        CheckCapacity();
        ++m_Size;
        return &m_pData[ m_Size - 1 ];
    }

    void AddZero( u32 count = 1 )
    {
        assert( count > 0 );

        u32 newSize = m_Size + count;
        u32 newCapacity = m_Capacity;
        while( EnhancePolicy::ShouldEnhance( newSize , newCapacity ) )
        {
            newCapacity = EnhancePolicy::EnhanceCapacity( newCapacity );
        }

        m_Capacity = newCapacity;
        m_pData = static_cast<ElementType*>( m_pAllocator->Realloc( m_pData , m_Capacity * sizeof( ElementType ) ) );

        std::memset( m_pData + m_Size , 0 , count * sizeof(ElementType) );
        m_Size += count;
    }


    void AddNum( u32 count )
    {
        u32 newSize = m_Size + count;
        u32 newCapacity = m_Capacity;
        while( EnhancePolicy::ShouldEnhance( newSize , newCapacity ) )
        {
            newCapacity = EnhancePolicy::EnhanceCapacity( newCapacity );
        }

        m_Capacity = newCapacity;
        m_pData = static_cast<ElementType*>( m_pAllocator->Realloc( m_pData , m_Capacity * sizeof( ElementType ) ) );

        while( m_Size < newSize )
        {
            m_pData[ m_Size ] = ElementType();
            ++m_Size;
        }
    }

    template< typename OtherElementType >
    void AddRange( const OtherElementType* pData , u32 count , u32 startIdx = -1 )
    {
        u32 newCapacity = EnhancePolicy::CalculateProperCapacity( m_Size + count );
        if ( newCapacity !=  m_Capacity )
        {
            m_Capacity = newCapacity;
            m_pData = static_cast<ElementType*>( m_pAllocator->Realloc( m_pData , m_Capacity * sizeof( ElementType ) ) );
        }

        if( startIdx == -1 )
            startIdx = m_Size;

        assert( startIdx <= m_Size );

        // move data from start index to the end
        u32 destIdx = startIdx + count;
        u32 srcIdx = startIdx;
        while( srcIdx < m_Size )
        {
            m_pData[ destIdx ] = m_pData[ srcIdx ];
            ++destIdx;
            ++srcIdx;
        }


        for( u32 i = 0 ; i < count ; ++i )
        {
            m_pData[ startIdx + i ] = pData[ i ];
        }

        m_Size += count;
    }

    template< typename OtherElementType >
    void AddRange( const Array<OtherElementType>& other , u32 startIdx = -1 )
    {
        AddRange( other.GetData() , other.GetSize() , startIdx );
    }

    template< typename OtherElementType >
    void Append( const Array<OtherElementType>& other )
    {
        AddRange( other , -1 );
    }

    template< typename OtherElementType >
    void Insert( const OtherElementType& element , u32 index )
    {
        AddRange( &element , 1 , index );
    }

    template< typename OtherElementType >
    ElementType PushBack( const OtherElementType& element )
    {
        ElementType newElement =  element;
        Insert( newElement , -1 );
        return newElement;
    }

    template< typename OtherElementType >
    ElementType PushFront( const OtherElementType& element )
    {
        ElementType newElement =  element;
        Insert( newElement , 0 );
        return newElement;
    }

    ElementType PopBack()
    {
        ElementType lastElement = GetLast();
        RemoveLast();
        return lastElement;
    }

    void RemoveLast()
    {
        RemoveAt( m_Size - 1 );
    }


    void RemoveAt( u32 index )
    {
        RemoveInRange( index , 1 );
    }


    void RemoveAll()
    {
        RemoveInRange( 0 , m_Size );
    }


    void RemoveInRange( u32 index , u32 count = -1 )
    {
        u32 removeCount = count == -1 ? m_Size - index : count;
        if( removeCount == 0 ) return;

        assert( removeCount <= m_Size );
        u32 startIdx = index;
        u32 lastIdx = startIdx + removeCount - 1;
        assert( startIdx < m_Size );
        assert ( lastIdx < m_Size );

        for ( u32 i = startIdx; i <= lastIdx; ++i )
        {
            m_pData[ i ].~ElementType();
        }

        u32 destIdx = startIdx;
        u32 srcIdx = lastIdx + 1;
        while( srcIdx < m_Size )
        {
            m_pData[ destIdx ] = m_pData[ srcIdx ];
            ++destIdx;
            ++srcIdx;
        }

        m_Size -= removeCount;

        while( EnhancePolicy::ShouldShrink( m_Size , m_Capacity ) )
        {
            Shrink();
        }
    }


    void Clear()
    {
        RemoveAll();
    }



    u32 Size() const { return m_Size; }
    u32 Capacity() const { return m_Capacity; }
    u32 GetSize() const { return m_Size; }
    u32 GetCapacity() const { return m_Capacity; }
    bool IsEmpty() const { return m_Size == 0; }
    ElementType GetFirst() const { assert( !IsEmpty() ); return m_pData[0]; }
    ElementType GetLast() const { assert( !IsEmpty() ); return m_pData[ m_Size - 1 ]; }

    const ElementType* GetData() const { return m_pData; }
    ElementType* GetData() { return m_pData; }


    u32 Find( const ElementType& element ) const
    {
        for ( int i = 0; i < m_Size; ++i )
        {
            if ( m_pData[i] == element )
            {
                return i;
            }
        }
        return -1;
    }

    template< typename Predicate >
    u32 Find( Predicate predicate ) const
    {
        for( int i = 0 ; i < m_Size ; ++i )
        {
            if( predicate( m_pData[i] ) )
                return i;
        }
        return -1;
    }

    ElementType& operator[]( IndexType index )
    {
        assert( IsValidIndex( index ) );
        return m_pData[ index ];
    }

    const ElementType& operator[]( IndexType index ) const
    {
        assert(IsValidIndex( index ) );
        return m_pData[ index ];
    }


    Array& operator=( const Array& other )
    {
        Dispose();

        m_pAllocator = &Allocator::GetInstance();
        m_Size = other.m_Size;
        m_Capacity = other.m_Capacity;
        m_pData = static_cast<ElementType*>( m_pAllocator->Allocate( m_Capacity * sizeof(ElementType) ) );
        memcpy( m_pData , other.m_pData , m_Size * sizeof(ElementType) );
        return *this;
    }


    template<typename OtherElementType>
    Array& operator=( const Array<OtherElementType>& other )
    {
        Dispose();

        m_pAllocator = &Allocator::GetInstance();
        m_Size = other.m_Size;
        m_Capacity  = other.m_Capacity;
        m_pData = static_cast<ElementType*>( m_pAllocator->Allocate( m_Capacity * sizeof(ElementType) ) );

        for (int i = 0; i < m_Size; ++i)
        {
            m_pData[i] = other.m_pData[i];
        }

        return *this;
    }

    Array& operator=( Array&& other )
    {
        Dispose();

        m_pAllocator = other.m_pAllocator;
        m_Size = other.m_Size;
        m_Capacity = other.m_Capacity;
        m_pData = other.m_pData;

        other.m_pAllocator = nullptr;
        other.m_pData = nullptr;
        other.m_Size = 0;
        other.m_Capacity = 0;

        return *this;
    }

    Array& operator=( const std::initializer_list<ValueType>& _list )
    {
        Dispose();

        m_pAllocator = &Allocator::GetInstance();
        m_Size = _list.size();
        m_Capacity = EnhancePolicy::CalculateProperCapacity( m_Size );
        m_pData = static_cast<ElementType*>( m_pAllocator->Allocate( m_Capacity * sizeof(ElementType) ) );
        u32 i = 0;
        for( const auto & element : _list )
        {
            m_pData[i] = element;
            ++i;
        }

        return *this;
    }


private:
    void CheckCapacity()
    {
        if( EnhancePolicy::ShouldEnhance( m_Size , m_Capacity ) )
        {
            Enhance();
        }
        else if( EnhancePolicy::ShouldShrink( m_Size , m_Capacity ) )
        {
            Shrink();
        }
    }

    void Enhance()
    {
        m_Capacity = EnhancePolicy::EnhanceCapacity( m_Capacity );
        m_pData = static_cast<ElementType*>( m_pAllocator->Realloc( m_pData , m_Capacity * sizeof( ElementType ) ) );
    }

    void Shrink()
    {
        m_Capacity = EnhancePolicy::ShrinkCapacity( m_Capacity );
        m_pData = static_cast<ElementType*>( m_pAllocator->Realloc( m_pData , m_Capacity * sizeof( ElementType ) ) );
    }

    void Dispose()
    {
        for ( u32 i = 0; i < m_Size; ++i )
        {
            m_pData[i].~ElementType();
        }
        if( m_pData )
            m_pAllocator->Free( m_pData );
        m_pData = nullptr;
        m_Size = 0;
        m_Capacity = 0;
    }


private:
    ElementType *m_pData;
    u32 m_Size;
    u32 m_Capacity;
    Allocator *m_pAllocator;
};


}

#endif //XLUA_ARRAY_H
