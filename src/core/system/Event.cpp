//
// Created by Harry on 9/20/2023.
//

#include "Event.h"
#include <cassert>
#if PLATFORM_IS_WINDOWS
#include <windows.h>
#endif

namespace LXX
{
namespace System
{


Event::Event()
: _handle( 0 )
{

}


Event::~Event()
{
    Destroy();
}


bool Event::Create(bool bManualReset, bool bInitialSinged )
{
#if PLATFORM_IS_WINDOWS
    _handle = reinterpret_cast<u64>( CreateEventA( NULL , bManualReset , bInitialSinged , NULL ) );
#endif

    return _handle != 0;
}


void Event::Destroy()
{
    if( _handle != 0 )
    {
#if PLATFORM_IS_WINDOWS
        CloseHandle( reinterpret_cast<HANDLE>( _handle ) );
#endif
    }
    _handle = 0;
}


void Event::WaitForEvent( u32 milliSecond )
{
    assert( IsValid() );
#if PLATFORM_IS_WINDOWS
    WaitForSingleObject( reinterpret_cast<HANDLE>(_handle) , milliSecond );
#endif
}

}
} // LXX