//
// Created by Harry on 8/26/2023.
//

#ifndef XLUA_LIST_H
#define XLUA_LIST_H
#include <cassert>
#include <initializer_list>
#include "Iterator.h"
#include <core/mem/MemoryAllocator.h>

namespace LXX
{


template < typename ElementType >
struct SingleListNode
{
    struct IndexType
    {
        IndexType( SingleListNode *pointer , SingleListNode *head , SingleListNode *tail )
        {
            this->pointer = pointer;
            this->head = head;
            this->tail = tail;
        }

        SingleListNode *pointer;
        SingleListNode *head;
        SingleListNode *tail;

        void OffsetIndex( s32 offset )
        {
            if( offset <=  0 ) return;

            while( offset > 0 && pointer != nullptr )
            {
                pointer = pointer->Next;
                offset--;
            }
        }
    };

    ElementType Value;
    SingleListNode *Next;
};

template < typename ElementType >
struct BidirectionalListNode
{
    struct IndexType
    {
        IndexType( BidirectionalListNode *pointer , BidirectionalListNode *head , BidirectionalListNode *tail )
        {
            this->pointer = pointer;
            this->head = head;
            this->tail = tail;
        }

        BidirectionalListNode *pointer;
        BidirectionalListNode *head;
        BidirectionalListNode *tail;

        bool operator== ( const IndexType &other ) const
        {
            return pointer == other.pointer;
        }

        void OffsetIndex( s32 offset )
        {
            if ( offset > 0 )
            {
                while( offset > 0 && pointer != nullptr )
                {
                    pointer = pointer->Next;
                    offset--;
                }
            }
            else if ( offset < 0 )
            {
                while( offset < 0 && pointer != nullptr )
                {
                    pointer = pointer->Prev;
                    offset++;
                }
            }
        }
    };

    ElementType Value;
    BidirectionalListNode *Prev;
    BidirectionalListNode *Next;
};

template < typename ElementType , typename Node =  BidirectionalListNode< ElementType >,  typename Allocator = MemoryAllocator >
class List
{
    typedef Node NodeType;
    friend class Iterator< List >;
    friend class ReverseIterator< List >;
    friend class ConstIterator< List >;
public:

    typedef typename NodeType::IndexType IndexType;
    typedef ElementType ValueType;
    typedef ValueType* ValueTypePointer;
    typedef ValueType& ValueTypeReference;
    typedef s32 DifferenceType;

    typedef Iterator< List > IteratorType;
    typedef ReverseIterator< List > ReverseIteratorType;
    typedef ConstIterator< List > ConstIteratorType;

    bool IsValidIndex( const IndexType& index ) const
    {
        return index.pointer  != nullptr;
    }
    void OffsetIndex( IndexType &index , s32 offset )
    {
        index.OffsetIndex( offset );
    }

    IteratorType Begin() { return IteratorType( this , IndexType( m_pHead , m_pHead , m_pTail ) ); }
    IteratorType End() { return IteratorType( this , IndexType( nullptr , m_pHead , m_pTail ) ); }
    ConstIteratorType ConstBegin() const { return ConstIteratorType ( this , IndexType( m_pHead , m_pHead , m_pTail ) ); }
    ConstIteratorType ConstEnd() const { return ConstIteratorType ( this , IndexType( nullptr , m_pHead , m_pTail ) ); }
    ReverseIteratorType ReverseBegin() { return ReverseIteratorType ( this , IndexType( m_pTail , m_pHead , m_pTail ) ); }
    ReverseIteratorType  ReverseEnd() { return ReverseIteratorType ( this , IndexType( nullptr , m_pHead , m_pTail ) ); }

    IteratorType begin() { return Begin(); }
    IteratorType end() { return End(); }
    ConstIteratorType cbegin() const { return ConstBegin(); }
    ConstIteratorType cend() const { return ConstEnd(); }
    ReverseIteratorType  rbegin() { return ReverseBegin(); }
    ReverseIteratorType  rend() { return ReverseEnd(); }

    List()
    : m_pHead( nullptr )
    , m_pTail( nullptr )
    , m_Size( 0 )
    , m_pAllocator( &Allocator::GetInstance() )
    {

    }

    template<typename OtherElementType>
    List(  const List< OtherElementType >& other )
        : m_pHead( nullptr )
        , m_pTail( nullptr )
        , m_Size( 0 )
        , m_pAllocator( &Allocator::GetInstance() )
    {
        for ( const auto & element : other )
        {
            PushBack( element ) ;
        }
    }


    template<typename OtherElementType>
    List( const std::initializer_list<OtherElementType>& other )
            : m_pHead( nullptr )
            , m_pTail( nullptr )
            , m_Size( 0 )
            , m_pAllocator( &Allocator::GetInstance() )
    {
        for( const auto & element : other )
        {
            PushBack( element );
        }
    }

    ~List()
    {
        Dispose();
    }

    u32 GetSize() const { return m_Size; }
    u32 Size() const { return m_Size; }

    void Clear() { Dispose(); }

    template< typename OtherElementType >
    void PushBack( const OtherElementType& Value )
    {
        NodeType *newNode = new ( m_pAllocator->Allocate( sizeof( NodeType ) ) ) NodeType();
        newNode->Value = Value;
        newNode->Next = nullptr;
        ++ m_Size;

        if( m_pHead == nullptr )
        {
            assert( m_pTail == nullptr );
            m_pHead = m_pTail = newNode;
        }
        else
        {
            m_pTail->Next = newNode;
            m_pTail = newNode;
        }
    }


    void PushFront( const ElementType& Value )
    {
        NodeType *newNode = new ( m_pAllocator->Allocate( sizeof( NodeType ) ) ) NodeType();
        newNode->Value = Value;
        newNode->Next = nullptr;
        ++ m_Size;

        if( m_pHead == nullptr )
        {
            assert( m_pTail == nullptr );
            m_pHead = m_pTail = newNode;
        }
        else
        {
            newNode->Next = m_pHead;
            m_pHead = newNode;
        }
    }

    void Remove( const ElementType& Value , bool bAll  = true )
    {
        NodeType *Pre = nullptr;
        NodeType *Cur = m_pHead;
        while ( Cur )
        {
            NodeType *Next = Cur->Next;

            if( Cur->Value == Value )
            {
               if( Pre ) Pre->Next = Next;
               else m_pHead = Next;

               if( Next == nullptr ) m_pTail = Pre;

               m_pAllocator->Free( Cur );
               -- m_Size;
               if( !bAll ) break;

               Cur = Next;
               continue;
            }

            Pre = Cur;
            Cur = Next;
        }
    }

    ElementType & operator[]( IndexType index )
    {
        assert( IsValidIndex( index ) );
        return index.pointer->Value;
    }

    ElementType operator[]( IndexType index ) const
    {
        assert( IsValidIndex( index ) );
        return index.pointer->Value;
    }
private:
    void Dispose()
    {
        NodeType *Cur = m_pHead;
        while ( Cur )
        {
            NodeType *Next = Cur->Next;
            m_pAllocator->Free( Cur );
            Cur = Next;
        }

        m_pHead = nullptr;
        m_pTail = nullptr;
        m_Size = 0;
    }


private:
    Allocator *m_pAllocator;
    NodeType *m_pHead;
    NodeType *m_pTail;
    u32 m_Size;
};


} // LXX

#endif //XLUA_LIST_H
