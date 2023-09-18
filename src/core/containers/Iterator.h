//
// Created by Harry on 8/26/2023.
//

#ifndef XLUA_ITERATOR_H
#define XLUA_ITERATOR_H
#include <iterator>
#include <core/LXX.h>

namespace LXX
{

template<typename Container>
class Iterator
{
    friend Container;
public:
    typedef Container ContainerType;
    typedef typename ContainerType::ValueType ValueType;
    typedef typename ContainerType::DifferenceType DifferenceType;
    typedef typename ContainerType::ValueTypePointer Pointer;
    typedef typename ContainerType::ValueTypeReference Reference;
    typedef typename ContainerType::IndexType IndexType;


    Iterator()
            : m_pArray( nullptr )
            ,  m_Index()
    {

    }

    Iterator( ContainerType * array , IndexType index )
            : m_pArray( array )
            , m_Index( index )
    {

    }

    Iterator( const Iterator &other )
            :  m_pArray( other.m_pArray )
            , m_Index( other.m_Index )
    {

    }

    Iterator& operator++()
    {
        m_pArray->Container::OffsetIndex( m_Index , 1 );
        return *this;
    }

    Iterator operator++( int )
    {
        Iterator temp = *this;
        ++ (*this);
        return temp;
    }

    Iterator& operator--()
    {
        m_pArray->Container::OffsetIndex( m_Index , -1 );
        return *this;
    }

    Iterator operator--( int )
    {
        Iterator temp = *this;
        -- (*this);
        return temp;
    }


    bool operator==( const Iterator& other ) const
    {
        return m_Index == other.m_Index;
    }

    bool operator!=( const Iterator& other ) const
    {
        return !( *this == other );
    }

    const ValueType& operator*() const
    {
        return m_pArray->Container::operator[]( m_Index );
    }

    ValueType& operator*()
    {
        return m_pArray->Container::operator[]( m_Index );
    }

    Pointer operator->()
    {
        return &m_pArray->Container::operator[]( m_Index );
    }

    const Pointer operator->() const
    {
        return &m_pArray->Container::operator[]( m_Index );
    }

    DifferenceType operator-( const Iterator& other ) const
    {
        return m_Index - other.m_Index;
    }

    Iterator operator+( const DifferenceType& diff ) const
    {
        Iterator temp = *this;
        temp.m_pArray->Container::OffsetIndex( temp.m_Index , diff );
        return temp;
    }

    Iterator operator-( const DifferenceType& diff ) const
    {
        return operator+( -diff );
    }

    Iterator& operator+=( const DifferenceType& diff )
    {
        m_pArray->Container::OffsetIndex( m_Index , diff );
        return *this;
    }

    Iterator& operator-=( const DifferenceType& diff )
    {
        return operator+=(-diff);
    }

    operator bool() const
    {
        return IsValid();
    }

    constexpr bool IsValid() const
    {
        return  m_pArray->Container::IsValidIndex( m_Index );
    }
private:
    ContainerType* m_pArray;
    IndexType m_Index;
};


template< typename Container >
class ConstIterator
{
    friend Container;
public:
    typedef Container ContainerType;
    typedef typename ContainerType::ValueType ValueType;
    typedef typename ContainerType::DifferenceType DifferenceType;
    typedef typename ContainerType::ValueTypePointer Pointer;
    typedef typename ContainerType::ValueTypeReference Reference;
    typedef typename ContainerType::IndexType IndexType;


public:
    ConstIterator( const ContainerType * array , IndexType index )
            : m_pArray( array )
            , m_Index( index )
    {

    }

    ConstIterator( const  ConstIterator& other )
            : m_pArray( other.m_pArray )
            ,  m_Index( other.m_Index )
    {

    }

    ConstIterator& operator++()
   {
       ++m_Index;
       assert( m_Index <= m_pArray->m_Size );
       return *this;
   }

    ConstIterator operator++( int )
                               {
                               Iterator temp = *this;
                               ++ (*this);
                               return temp;
                               }

    ConstIterator& operator--()
                           {
                           -- m_Index;
                           assert( m_Index < m_pArray->m_Size && m_Index >= 0 );
                           return *this;
                           }


    ConstIterator operator--( int )
                               {
                               Iterator temp = *this;
                               -- (*this);
                               return temp;
                               }


    bool operator==( const ConstIterator& other ) const
                                                   {
                                                   return m_Index == other.m_Index;
                                                   }


    bool operator!=( const ConstIterator& other ) const
                                                   {
                                                   return m_Index != other.m_Index;
                                                   }

    ValueType operator*() const
    {
        return m_pArray->Container::operator[]( m_Index );
    }

    const Pointer operator->() const
    {
        return &m_pArray->Container::operator[]( m_Index );
    }
private:
    const ContainerType* m_pArray;
    IndexType m_Index;
};


template<typename Container>
class ReverseIterator
{
    friend Container;
public:
    typedef Container ContainerType;
    typedef typename ContainerType::ValueType ValueType;
    typedef typename ContainerType::DifferenceType DifferenceType;
    typedef typename ContainerType::ValueTypePointer Pointer;
    typedef typename ContainerType::ValueTypeReference Reference;
    typedef typename ContainerType::IndexType IndexType;


    ReverseIterator()
        : m_pArray( nullptr )
        ,  m_Index()
    {

    }

    ReverseIterator( ContainerType * array , IndexType index )
        : m_pArray( array )
        , m_Index( index )
    {

    }

    ReverseIterator( const ReverseIterator &other )
            :  m_pArray( other.m_pArray )
            , m_Index( other.m_Index )
    {

    }

    ReverseIterator& operator++()
    {
        m_pArray->Container::OffsetIndex( m_Index , -1 );
        return *this;
    }

    ReverseIterator operator++( int )
    {
        Iterator temp = *this;
        ++ (*this);
        return temp;
    }

    ReverseIterator& operator--()
    {
        m_pArray->Container::OffsetIndex( m_Index , 1 );
        return *this;
    }

    ReverseIterator operator--( int )
    {
        Iterator temp = *this;
        -- (*this);
        return temp;
    }


    bool operator==( const ReverseIterator& other ) const
    {
        return m_Index == other.m_Index;
    }

    bool operator!=( const ReverseIterator& other ) const
    {
        return !( *this == other );
    }

    const ValueType& operator*() const
    {
        return m_pArray->Container::operator[]( m_Index - 1 );
    }

    ValueType& operator*()
    {
        return m_pArray->Container::operator[]( m_Index - 1 );
    }

    Pointer operator->()
    {
        return &m_pArray->Container::operator[]( m_Index - 1 );
    }

    const Pointer operator->() const
    {
        return &m_pArray->Container::operator[]( m_Index - 1 );
    }

    DifferenceType operator-( const ReverseIterator& other ) const
    {
        return other.m_Index - m_Index ;
    }

    ReverseIterator operator+( const DifferenceType& diff ) const
    {
        Iterator temp = *this;
        temp.m_pArray->Container::OffsetIndex( temp.m_Index , -diff );
        return temp;
    }

    ReverseIterator operator-( const DifferenceType& diff ) const
    {
        return operator+( -diff );
    }

    ReverseIterator& operator+=( const DifferenceType& diff )
    {
        m_pArray->Container::OffsetIndex( m_Index , diff );
        return *this;
    }

    ReverseIterator& operator-=( const DifferenceType& diff )
    {
        return operator+=(-diff);
    }

    operator bool() const
    {
        return IsValid();
    }

    constexpr bool IsValid() const
    {
        return  m_pArray->Container::IsValidIndex( m_Index - 1 );

    }
private:
    ContainerType* m_pArray;
    IndexType m_Index;
};


} // namespace LXX

#endif //XLUA_ITERATOR_H
