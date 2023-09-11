//
// Created by Harry on 8/31/2023.
//

#ifndef XLUA_HASHBUCKET_H
#define XLUA_HASHBUCKET_H
#include <core/mem/MemoryAllocator.h>

namespace LXX
{

template< typename KeyType , typename ValueType, u32 InitBucketCount = 16 ,typename Allocator = MemoryAllocator >
class HashBucket
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

    HashBucket()
            : m_Size( 0 )
            , m_pAllocator( &Allocator::GetInstance() )
            , m_SlotCount( InitBucketCount )
    {
        m_pSlot = (Slot **)m_pAllocator->Allocate( m_SlotCount * sizeof(Slot*) );
        memset( m_pSlot , 0 , m_SlotCount * sizeof(Slot*) );
    }
    ~HashBucket()
    {
        Clear();
        m_pAllocator->Free( m_pSlot );
        m_pSlot = nullptr;
    }
    u32 Size() const { return m_Size; }
    u32 GetSlotCount() const { return m_SlotCount; }
    Slot** GetSlotBase() { return m_pSlot; }

    bool TryGet( const KeyType &key , ValueType &outValue ) const
    {
        u32 hashValue = Hash( key );
        u32 slotIdx = hashValue % m_SlotCount;
        Slot *slot = m_pSlot[slotIdx];

        while ( slot )
        {
            if ( IsEqual( slot->data, key ) )
            {
                outValue = slot->data;
                return true;
            }

            slot = slot->next;
        }

        return false;
    }

    ValueType* Add( const KeyType &key , const ValueType &value )
    {
        Slot *dataSlot = new( m_pAllocator->Allocate( sizeof( Slot ) ) ) Slot();
        dataSlot->data = value;

        u32 hashValue = Hash( key );
        u32 bucketIndex = hashValue % m_SlotCount;
        Slot *slot = m_pSlot[bucketIndex];

        HashImpl( dataSlot , m_pSlot , m_SlotCount );
        ++ m_Size;

        CheckIfNeedRehash();

        return &dataSlot->data;
    }

    void Remove( const KeyType &key , const ValueType &value )
    {
        u32 hashValue = Hash( key );
        u32 bucketIndex = hashValue % m_SlotCount;
        Slot *slot = m_pSlot[bucketIndex];
        Slot *preSlot = nullptr;
        Slot *dataSlotToRemove = nullptr;
        while ( slot )
        {
            if( IsEqual( slot->data, value ) )
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

        for( u32 i = 0 ; i < m_SlotCount ; ++i )
        {
            Slot *slot = m_pSlot[i];
            while ( slot )
            {
                HashImpl( slot, newSlot , newSlotCount );
                slot = slot->next;
            }
        }

        m_pAllocator->Free( m_pSlot );
        m_pSlot = newSlot;
    }

    void HashImpl( Slot* dataSlot , Slot** slot , u32 slotCount )
    {
        u32 hashValue = Hash( dataSlot->data );
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


} // namespace LXX

#endif //XLUA_HASHBUCKET_H
