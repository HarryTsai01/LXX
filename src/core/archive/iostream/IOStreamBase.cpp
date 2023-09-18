//
// Created by Harry on 9/15/2023.
//

#include "IOStreamBase.h"

namespace LXX
{


IOStreamBase::IOStreamBase( ArchiveBase& archive )
: _archive( archive )
{

}


void IOStreamBase::Serialize( void* data, size_t size )
{
    _archive.Serialize( data, size );
}


void IOStreamBase::Serialize( const void* data, size_t size )
{
    _archive.Serialize( data, size );
}


} // LXX