//
// Created by Harry on 9/20/2023.
//

#ifndef XLUA_EVENT_H
#define XLUA_EVENT_H
#include <core/LXX.h>

namespace LXX
{
namespace System
{


class Event
{
public:
    Event();
    ~Event();
    u64 GetHandle() const { _handle; }
    bool IsValid() const { return _handle != 0 ; }
    bool Create( bool bManualReset = false , bool bInitialSinged = false );
    void Destroy();

    void WaitForEvent( u32 milliSecond = -1 );
private:
    u64 _handle;
};


}

} // LXX

#endif //XLUA_EVENT_H
