//
// Created by Harry on 8/7/2023.
//

#ifndef XLUA_GC_H
#define XLUA_GC_H
#include <chrono>
#include "GCObject.h"

using namespace std;

namespace LXX
{

class GC
{
public:
    enum class Phrase : u8
    {
        Restart ,
        Mark ,
        Sweep ,
        WaitForGC ,
    };
public:
    GC();
    ~GC();
    static GC& GetInstance();

    void Initialize();
    void Destroy();
    void Tick();
    void ForceGC();
    void AddObject( GCObject *Object );

private:
    Phrase CurrentPhrase;
    GCObject *PreviousObjectHeadBeforeGC;
    GCObject *ObjectHeadBeforeGC;
    GCObject *ObjectHead;
    GCObject *GrayHead;
    using Clock = std::chrono::high_resolution_clock;
    using TimePoint = std::chrono::time_point<Clock>;
    TimePoint lastGCTime;

    static GC* m_pInstance;

};

} // LXX

#endif //XLUA_GC_H
