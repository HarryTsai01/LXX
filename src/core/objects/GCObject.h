//
// Created by Harry on 8/7/2023.
//

#ifndef XLUA_GCOBJECT_H
#define XLUA_GCOBJECT_H
#include <vector>
#include <core/LXX.h>
#include <core/mem/MemoryAllocator.h>

namespace LXX
{

#define GC_OBJECT_MARK_COLOR_MASK_WHITE 0x01
#define GC_OBJECT_MARK_COLOR_MASK_GRAY 0x02
#define GC_OBJECT_MARK_COLOR_MASK_BLACK 0x04
#define GC_OBJECT_MARK_CREATE_DURING_GC 0x08

using namespace std;
class GCObjectCollector;


class GCObject
{
    friend class GC;
public:
    GCObject();
    virtual ~GCObject();

    void Mark( u32 flags );
    void ClearMark( u32 mask );
    void MarkAsWhite()
    {
        ClearMark( GC_OBJECT_MARK_COLOR_MASK_WHITE | GC_OBJECT_MARK_COLOR_MASK_GRAY |  GC_OBJECT_MARK_COLOR_MASK_BLACK );
        Mark( GC_OBJECT_MARK_COLOR_MASK_WHITE );
    }
    void MarkAsGray()
    {
        ClearMark( GC_OBJECT_MARK_COLOR_MASK_WHITE | GC_OBJECT_MARK_COLOR_MASK_GRAY |  GC_OBJECT_MARK_COLOR_MASK_BLACK );
        Mark( GC_OBJECT_MARK_COLOR_MASK_GRAY );
    }
    void MarkAsBlack()
    {
        ClearMark( GC_OBJECT_MARK_COLOR_MASK_WHITE | GC_OBJECT_MARK_COLOR_MASK_GRAY |  GC_OBJECT_MARK_COLOR_MASK_BLACK );
        Mark( GC_OBJECT_MARK_COLOR_MASK_BLACK );
    }

    bool IsMarkedAsWhite() const
    {
        return ( marks & GC_OBJECT_MARK_COLOR_MASK_WHITE ) != 0;
    }

    bool IsMarkedAsGray() const
    {
        return ( marks & GC_OBJECT_MARK_COLOR_MASK_GRAY ) != 0;
    }

    bool IsMarkedAsBlack() const
    {
        return ( marks & GC_OBJECT_MARK_COLOR_MASK_BLACK ) != 0;
    }

    bool IsMarkedAsNewCreatedDuringGC() const
    {
        return ( marks & GC_OBJECT_MARK_CREATE_DURING_GC ) != 0;
    }

    virtual void CollectReferences( GCObjectCollector& collector ) { }


    template<typename T>
    T* Cast()
    {
        return dynamic_cast<T*>( this );
    }

    template<typename T>
    const T* Cast() const
    {
        return dynamic_cast<const T*>( this );
    }

    template<typename T>
    bool IsA()
    {
        return Cast<T>() != nullptr;
    }

    template<typename T>
    bool IsA() const
    {
        return Cast<T>() != nullptr;
    }

protected:
    GCObject* allNext;
    GCObject* grayNext;
    u32 marks;
};

template<typename T>
T* Cast( GCObject* obj )
{
    return obj ? obj->Cast<T>() : nullptr;
}


} // namespace LXX





#endif //XLUA_GCOBJECT_H
