//
// Created by Harry on 8/24/2023.
//

#ifndef XLUA_UNORDERMAP_H
#define XLUA_UNORDERMAP_H
#include <core/Containers/Array.h>
#include <core/Containers/HashUtility.h>
#include "Iterator.h"

namespace LXX
{



template<typename Key, typename Value , u32 InitBucketCount = 16, typename Allocator = MemoryAllocator >
class UnorderedMap
{
    friend class Iterator< UnorderedMap >;
    friend class ReverseIterator< UnorderedMap >;
    friend class ConstIterator< UnorderedMap >;
public:

    struct Pair
    {
        Key First;
        Value Second;
    };

    typedef Pair ValueType;
    typedef ValueType* ValueTypePointer;
    typedef ValueType& ValueTypeReference;

    typedef Iterator< UnorderedMap > IteratorType;
    typedef ConstIterator< UnorderedMap > ConstIteratorType;

private:
    struct IndexType;
    class Bucket
    {
    public:
        struct Slot
        {
            Slot()
            : next( nullptr )
            {

            }
            ValueType data;
            Slot *next;
        };

        Bucket()
            : m_Size( 0 )
            , m_pAllocator( &Allocator::GetInstance() )
            , m_SlotCount( InitBucketCount )
        {
            m_pSlot = (Slot **)m_pAllocator->Allocate( m_SlotCount * sizeof(Slot*) );
            memset( m_pSlot , 0 , m_SlotCount * sizeof(Slot*) );
        }
        Bucket( const Bucket &rhs ) = delete;
        Bucket( Bucket &&rhs )
        {
            m_Size = rhs.m_Size;
            m_pAllocator = rhs.m_pAllocator;
            m_SlotCount = rhs.m_SlotCount;
            m_pSlot = rhs.m_pSlot;

            rhs.m_pSlot = nullptr;
            rhs.m_Size = 0;
            rhs.m_SlotCount = 0;
        }
        ~Bucket()
        {
            Clear();
            m_pAllocator->Free( m_pSlot );
            m_pSlot = nullptr;
        }

        Bucket &operator=( const Bucket &rhs ) = delete;
        Bucket &operator=( Bucket &&rhs )
        {
            m_Size = rhs.m_Size;
            m_pAllocator = rhs.m_pAllocator;
            m_SlotCount = rhs.m_SlotCount;
            m_pSlot = rhs.m_pSlot;
            rhs.m_pSlot = nullptr;
            rhs.m_Size = 0;
            rhs.m_SlotCount = 0;
            return *this;
        }

        u32 Size() const { return m_Size; }
        u32 GetSlotCount() const { return m_SlotCount; }
        Slot** GetSlotBase() { return m_pSlot; }

        const ValueType* Find( const Key &key , u32 *outSlotIdx = nullptr , Slot **outSlot = nullptr ) const
        {
            u32 hashValue = Hash( key );
            u32 slotIdx = hashValue % m_SlotCount;
            Slot *slot = m_pSlot[slotIdx];

            while ( slot )
            {
                if ( IsEqual( slot->data.First , key ) )
                {
                    if( outSlotIdx ) *outSlotIdx = slotIdx;
                    if( outSlot ) *outSlot = slot;
                    return &slot->data;
                }

                slot = slot->next;
            }

            return nullptr;
        }

        ValueType* Find( const Key &key , u32 *outSlotIdx = nullptr , Slot **outSlot = nullptr )
        {
            u32 hashValue = Hash( key );
            u32 slotIdx = hashValue % m_SlotCount;
            Slot *slot = m_pSlot[slotIdx];

            while ( slot )
            {
                if ( IsEqual( slot->data.First , key ) )
                {
                    if( outSlotIdx ) *outSlotIdx = slotIdx;
                    if( outSlot ) *outSlot = slot;
                    return &slot->data;
                }

                slot = slot->next;
            }

            return nullptr;
        }

        ValueType* Add( const Key &key , const Value &value )
        {
            Slot *dataSlot = new( m_pAllocator->Allocate( sizeof( Slot ) ) ) Slot();
            dataSlot->data.First = key;
            dataSlot->data.Second = value;

            HashImpl( dataSlot , m_pSlot , m_SlotCount );
            ++ m_Size;

            CheckIfNeedRehash();

            return &dataSlot->data;
        }

        void Remove( const Key &key )
        {
            u32 hashValue = Hash( key );
            u32 bucketIndex = hashValue % m_SlotCount;
            Slot *slot = m_pSlot[bucketIndex];
            Slot *preSlot = nullptr;
            Slot *dataSlotToRemove = nullptr;
            while ( slot )
            {
                if( IsEqual( slot->data.First , key ) )
                {
                    if( preSlot == nullptr )
                    {
                        dataSlotToRemove = slot;
                        m_pSlot[bucketIndex] = slot->next;
                    }
                    else
                    {
                        dataSlotToRemove = slot;
                        preSlot->next = slot->next;
                    }

                    break;
                }
                preSlot = slot;
                slot = slot->next;
            }

            if ( dataSlotToRemove )
            {
                m_pAllocator->Free( dataSlotToRemove );
                -- m_Size;
                CheckIfNeedRehash();
            }
        }

        void Clear()
        {
            for( u32 i = 0 ; i < m_SlotCount ; ++i )
            {
                Slot *slot = m_pSlot[i];
                while ( slot )
                {
                    Slot *next = slot->next;
                    m_pAllocator->Free( slot );
                    slot = next;
                }
                m_pSlot[i] = nullptr;
            }

            m_Size = 0;
            CheckIfNeedRehash();
        }
    private:
        void Rehash( u32 newSlotCount )
        {
            Slot **newSlot = (Slot **)m_pAllocator->Allocate( newSlotCount * sizeof( Slot* ) );
            memset( newSlot , 0 , newSlotCount * sizeof( Slot* ) );

            u32 count = 0;
            for( u32 i = 0 ; i < m_SlotCount ; ++i )
            {
                Slot *slot = m_pSlot[i];
                while ( slot )
                {
                    Slot* nextSlot = slot->next;
                    HashImpl( slot, newSlot , newSlotCount );
                    slot = nextSlot;
                    ++ count;
                }
            }

            m_pAllocator->Free( m_pSlot );
            m_pSlot = newSlot;
            m_SlotCount = newSlotCount;
        }

        void HashImpl( Slot* dataSlot , Slot** slot , u32 slotCount )
        {
            u32 hashValue = Hash( dataSlot->data.First );
            u32 bucketIndex = hashValue % slotCount;
            dataSlot->next = slot[bucketIndex];
            slot[bucketIndex] = dataSlot;
        }

        void CheckIfNeedRehash()
        {
            u32 newSlotCount = m_SlotCount;
           if(  m_Size > m_SlotCount )
           {
               newSlotCount = m_SlotCount * 2;
           }
           else if( m_Size < m_SlotCount / 3 && m_SlotCount > InitBucketCount )
           {
               newSlotCount = m_SlotCount / 2;
           }

           if( newSlotCount != m_SlotCount )
                Rehash( newSlotCount );
        }
    private:
        Slot **m_pSlot;
        u32 m_Size;
        u32 m_SlotCount;
        Allocator *m_pAllocator;
    };

    struct IndexType
    {
        IndexType( Bucket* bucket )
        : currentSlotIndex( 0 )
        , bucket( bucket )
        , currentSlot( nullptr )
        {
        }

        IndexType( Bucket* bucket , u32 slotIdx , typename Bucket::Slot* slot )
                : currentSlotIndex( slotIdx )
                , bucket( bucket )
                , currentSlot( slot )
        {
        }

        IndexType( const IndexType &other )
                : currentSlotIndex( other.currentSlotIndex )
                , bucket( other.bucket )
                , currentSlot( other.currentSlot )
        {

        }

        IndexType( IndexType &&other )
                : currentSlotIndex( other.currentSlotIndex )
                , bucket( other.bucket )
                , currentSlot( other.currentSlot )
        {
            other.currentSlotIndex = 0;
            other.bucket = nullptr;
            other.currentSlot = nullptr;
        }

        u32 currentSlotIndex;
        Bucket* bucket;
        typename Bucket::Slot *currentSlot;

        static IndexType Begin( Bucket* bucket )
        {
            IndexType index( bucket );
            auto slotBase = bucket->GetSlotBase();
            while ( index.currentSlotIndex < bucket->GetSlotCount() )
            {
                index.currentSlot = slotBase[ index.currentSlotIndex ] ;

                if( index.currentSlot ) break;

                ++ index.currentSlotIndex;
            }

            return index;
        }

        static IndexType End( Bucket* bucket )
        {
            IndexType index( bucket );
            index.currentSlotIndex = bucket->GetSlotCount();
            return index;
        }

        bool IsValid() const
        {
            return currentSlot != nullptr && currentSlotIndex < bucket->GetSlotCount() ;
        }

        void OffsetIndex( s32 offset )
        {
            assert( offset >= 0 );

            auto slotBase = bucket->GetSlotBase();

            while ( offset > 0 )
            {
                if( currentSlot->next )
                {
                    currentSlot = currentSlot->next;
                }
                else
                {
                    currentSlot = nullptr;
                    ++ currentSlotIndex;
                    while ( currentSlotIndex < bucket->GetSlotCount() )
                    {
                        currentSlot = slotBase[ currentSlotIndex ];
                        if( currentSlot ) break;
                        ++ currentSlotIndex;
                    }
                }
                -- offset;
            }

        }

        bool operator==( const IndexType &other ) const
        {
            return currentSlotIndex == other.currentSlotIndex && currentSlot == other.currentSlot;
        }
    };
    typedef s32 DifferenceType;


    bool IsValidIndex( const IndexType& index ) const
    {
        return index.IsValid();
    }
    void OffsetIndex( IndexType &index , s32 offset )
    {
        index.OffsetIndex( offset );
    }
public:
    IteratorType Begin() { return IteratorType( this , IndexType::Begin( &m_Bucket ) ); }
    IteratorType End() { return IteratorType( this , IndexType::End( &m_Bucket ) ) ; }
    ConstIteratorType ConstBegin() const { return ConstIteratorType ( this , IndexType::Begin( &m_Bucket ) ); }
    ConstIteratorType ConstEnd() const { return ConstIteratorType ( this , IndexType::End(  &m_Bucket ) ); }

    IteratorType begin() { return Begin(); }
    IteratorType end() { return End(); }
    ConstIteratorType cbegin() const { return ConstBegin(); }
    ConstIteratorType cend() const { return ConstEnd(); }

    UnorderedMap()
    {

    }
    UnorderedMap( const UnorderedMap& other )
    {
        for( const auto & element : other )
        {
            Add( element.First , element.Second );
        }
    }
    UnorderedMap( UnorderedMap&& other )
    :  m_Bucket( std::move( other.m_Bucket ) )
    {
    }

    UnorderedMap(const std::initializer_list< ValueType >& initializerList )
    {
        for( const auto & element : initializerList )
        {
            assert( !Contains( element.First ) );
            Add( element.First , element.Second );
        }
    }

    template< typename OtherKeyType , typename OtherValueType >
    UnorderedMap(const std::initializer_list< std::pair< OtherKeyType , OtherValueType > >& initializerList )
    {
        for( const auto & element : initializerList )
        {
            assert( !Contains( element.first ) );
            Add( element.first , element.second );
        }
    }

    UnorderedMap& operator=( const UnorderedMap& other )
    {
        Clear();
        for( const auto & element : other )
        {
            Add( element.First , element.Second );
        }
        return *this;
    }

    UnorderedMap& operator=( UnorderedMap&& other )
    {
        Clear();
        m_Bucket = std::move( other.m_Bucket );
        return *this;
    }

    Value* Add( const Key &key , const Value &value )
    {
        auto pair = m_Bucket.Find( key );
        if ( pair )
        {
            pair->Second = value;
            return &pair->Second;
        }
        else
        {
            return &m_Bucket.Add( key , value )->Second;
        }
    }

    void Remove( const Key &key )
    {
        m_Bucket.Remove( key );
    }

    bool Contains( const Key &key ) const
    {
        return m_Bucket.Find( key ) != nullptr;
    }

    const Value& operator[]( const Key &key ) const
    {
        auto pair = m_Bucket.Find( key );
        assert( pair != nullptr );
        return pair->Second;
    }

    Value& operator[]( const Key &key )
    {
        auto pair = m_Bucket.Find( key );
        if ( pair == nullptr )
        {
            pair = m_Bucket.Add( key , Value() );
        }
        return pair->Second;
    }

    ValueType operator[]( const IndexType &index ) const
    {
        assert( index.IsValid() );
        return ValueType( index.currentSlot->data.First , index.currentSlot->data.Second );
    }

    ValueType&  operator[]( const IndexType &index )
    {
        assert( index.IsValid() );
        return index.currentSlot->data;
    }

    u32 Size() const
    {
        return m_Bucket.Size();
    }

    IteratorType Find( const Key &key )
    {
        u32 slotIdx;
        typename Bucket::Slot * foundSlot;
        auto pair = m_Bucket.Find( key , &slotIdx , &foundSlot );
        if( pair == nullptr ) return IteratorType ( this , IndexType::End( &m_Bucket ) );

        return IteratorType( this , IndexType( &m_Bucket , slotIdx , foundSlot ) );
    }

    IteratorType Find( const Key &key ) const
    {
        u32 slotIdx;
        typename Bucket::Slot * foundSlot;
        auto pair = m_Bucket.Find( key , &slotIdx , &foundSlot );
        if( pair == nullptr ) return IteratorType ( this , IndexType::End( &m_Bucket ) );

        return IteratorType( this , IndexType( &m_Bucket , slotIdx , foundSlot ) );
    }

    void Clear()
    {
        m_Bucket.Clear();
    }

private:
    Bucket m_Bucket;
};

}

#endif //XLUA_UNORDERMAP_H
