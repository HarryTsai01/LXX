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
class Iterator :  public std::iterator<
            std::bidirectional_iterator_tag
            , typename Container::ValueType
            , typename Container::DifferenceType
            >
{
    friend Container;
public:
    typedef std::iterator_traits<Iterator> IteratorTraits;
    typedef typename IteratorTraits::iterator_category IteratorCategory;
    typedef typename IteratorTraits::value_type ValueType;
    typedef typename IteratorTraits::difference_type DifferenceType;
    typedef typename IteratorTraits::pointer Pointer;
    typedef typename IteratorTraits::reference Reference;
    typedef Container ContainerType;
    typedef typename Container::IndexType IndexType;

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
class ConstIterator :  public std::iterator<
        std::bidirectional_iterator_tag
        , const typename Container::ValueType
        , typename Container::DifferenceType
>
{
    friend Container;
public:
    typedef std::iterator_traits<ConstIterator> IteratorTraits;
    typedef typename IteratorTraits::iterator_category IteratorCategory;
    typedef typename IteratorTraits::value_type ValueType;
    typedef typename IteratorTraits::difference_type DifferenceType;
    typedef typename IteratorTraits::pointer Pointer;
    typedef typename IteratorTraits::reference Reference;
    typedef Container ContainerType;
    typedef typename Container::IndexType IndexType;

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


template< typename Container >
class ReverseIterator : public std::reverse_iterator< typename Container::IteratorType >
{
public:
    typedef Container ContainerType;
    typedef typename Container::IndexType IndexType;

    ReverseIterator( ContainerType* container , IndexType index )
    {
        std::reverse_iterator< typename Container::IteratorType >::current = Iterator( container , index );
    }
};

} // namespace LXX

#endif //XLUA_ITERATOR_H
