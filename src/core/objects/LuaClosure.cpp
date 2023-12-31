//
// Created by Harry on 9/2/2023.
//

#include "LuaClosure.h"
#include <core/Stack.h>

namespace LXX
{


LuaClosure::LuaClosure( ByteCodeChunk* chunk )
    : _chunk( chunk )
    , _parameterCount( 0 )
    , _parent( nullptr )
    , _name( nullptr )
{

}


void LuaClosure::AddLuaClosure( LuaClosure* closure )
{
    _children.PushBack( closure );
    closure->_parent = this;
}


} // LXX