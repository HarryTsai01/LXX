//
// Created by Harry on 8/16/2023.
//

#ifndef XLUA_GCOBJECTCOLLECTOR_H
#define XLUA_GCOBJECTCOLLECTOR_H
#include "GCObject.h"
#include <core/Containers/Array.h>

namespace LXX {


class GCObjectCollector
{
public:
    void AddObject( GCObject *object );
    void AddObjects( const Array<GCObject *> &objects );


    Array<GCObject *> m_objects;
};


} // LXX

#endif //XLUA_GCOBJECTCOLLECTOR_H
