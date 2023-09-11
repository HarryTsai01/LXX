//
// Created by Harry on 8/5/2023.
//
#include "GlobalState.h"

namespace LXX
{


GlobalState::GlobalState()
: MainThread( nullptr )
{
    _globalTable = new Table( );
}


State *GlobalState::NewState()
{
    return _states.PushBack( new State( this ) );
}


ByteCodeChunk *GlobalState::NewChunk( Array< String* > params )
{
    return _chunks.PushBack( new ByteCodeChunk( std::move( params) ) );
}


}
