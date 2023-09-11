//
// Created by Harry on 8/7/2023.
//

#include "GCObject.h"
#include "GC.h"

namespace LXX
{

GCObject::GCObject()
: marks(0)
, grayNext ( nullptr )
, allNext ( nullptr )
{
    GC::GetInstance().AddObject(this ) ;
}

GCObject::~GCObject()
{

}


void GCObject::Mark( u32 flags )
{
    marks |= flags;
}


void GCObject::ClearMark( u32 mask )
{
    marks &= ~mask;
}


}
