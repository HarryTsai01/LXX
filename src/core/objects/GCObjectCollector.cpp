//
// Created by Harry on 8/16/2023.
//

#include "GCObjectCollector.h"

namespace LXX
{

void GCObjectCollector::AddObject( GCObject *object )
{
    if( !object ) return;

    m_objects.PushBack( object );
}

void GCObjectCollector::AddObjects( const Array<GCObject *> &objects )
{
    m_objects.Append( objects );
}

} // LXX