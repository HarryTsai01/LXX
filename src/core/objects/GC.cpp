//
// Created by Harry on 8/7/2023.
//

#include "GC.h"
#include <cassert>
#include <core/config/Config.h>
#include <core/mem/MemoryAllocator.h>
#include <core/objects/GCObjectCollector.h>

namespace LXX
{


GC* GC::m_pInstance = nullptr;

GC::GC()
: CurrentPhrase ( Phrase::WaitForGC )
, ObjectHeadBeforeGC( nullptr )
, PreviousObjectHeadBeforeGC( nullptr )
, ObjectHead( nullptr )
, GrayHead( nullptr )
{

}

GC& GC::GetInstance()
{
    if( !m_pInstance ) m_pInstance = new GC();

    return *m_pInstance;
}


void GC::AddObject( GCObject *Object )
{
    Object->MarkAsWhite();
    if( CurrentPhrase !=  Phrase::WaitForGC )
    {
        Object->Mark( GC_OBJECT_MARK_CREATE_DURING_GC );
    }
    if ( ObjectHead )
    {
        Object->allNext = ObjectHead;
    }
    ObjectHead = Object;
}


void GC::Initialize()
{
    lastGCTime = std::chrono::system_clock::now();
}


void GC::Destroy()
{

}


void GC::Tick()
{
    if ( CurrentPhrase  == Phrase::WaitForGC )
    {
        TimePoint curTime = std::chrono::system_clock::now();
        std::chrono::duration<double> diff = curTime - lastGCTime;
        if ( diff.count() >= Config::GC_INTERVAL )
        {
            CurrentPhrase = Phrase::Restart;
            ObjectHeadBeforeGC = ObjectHead;
        }
    }
    else if ( CurrentPhrase == Phrase::Restart )
    {
        // collect object from stack and global state
    }
    else if ( CurrentPhrase == Phrase::Mark )
    {
        u32 markCount = 0;
        while ( GrayHead )
        {
            // pop object from gray list
            auto poppedObjectFromGray = GrayHead;
            assert( poppedObjectFromGray->IsMarkedAsWhite() );
            GrayHead = GrayHead->allNext;
            ++ markCount;
            if( markCount >= Config::GC_MARK_LIMIT )
                break;

            // collect all references from popped object
            GCObjectCollector collector;
            poppedObjectFromGray->CollectReferences( collector );
            poppedObjectFromGray->MarkAsBlack();

            for(  auto& ref : collector.m_objects )
            {
                if ( ref->IsMarkedAsWhite() && !ref->IsMarkedAsNewCreatedDuringGC() )
                {
                    ref->grayNext = GrayHead;
                    GrayHead = ref;
                }
            }
        }

        // when all objects are collected from gray list, start sweeping
        if(  GrayHead == nullptr )
        {
            GCObject* curObject = ObjectHead;
            while ( curObject )
            {
                if( curObject == ObjectHeadBeforeGC )
                    break;

                PreviousObjectHeadBeforeGC = curObject;
                curObject = curObject->allNext;
            }
            CurrentPhrase = Phrase::Sweep;
        }
    }
    else if ( CurrentPhrase == Phrase::Sweep )
    {
        // sweep all objects that are white and not marked new created during GC
        u32 sweepCount = 0;
        while( ObjectHeadBeforeGC )
        {
            GCObject* curObject = ObjectHeadBeforeGC;
            ObjectHeadBeforeGC = ObjectHeadBeforeGC->allNext;

            if ( curObject->IsMarkedAsWhite() && !curObject->IsMarkedAsNewCreatedDuringGC() )
            {
                PreviousObjectHeadBeforeGC->allNext = curObject->allNext;
                curObject->~GCObject();
                MemoryAllocator::GetInstance().Free( curObject );
                curObject = nullptr;
                ++ sweepCount;

                if(  sweepCount >= Config::GC_SWEEP_LIMIT )
                {
                    break;
                }
            }
            else
            {
                PreviousObjectHeadBeforeGC = PreviousObjectHeadBeforeGC->allNext;
            }
        }

        if ( !ObjectHeadBeforeGC )
        {
            CurrentPhrase = Phrase::WaitForGC;
            ObjectHeadBeforeGC = nullptr;
            PreviousObjectHeadBeforeGC  = nullptr;
            lastGCTime  = std::chrono::system_clock::now();
        }
    }
}


void GC::ForceGC()
{

}

} // LXX