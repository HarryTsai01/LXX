//
// Created by Harry on 9/15/2023.
//

#ifndef XLUA_IOSTREAMBASE_H
#define XLUA_IOSTREAMBASE_H
#include <core/archive/ArchiveBase.h>

namespace LXX
{

class IOStreamBase
{
public:
    IOStreamBase( ArchiveBase& archive );
    virtual ~IOStreamBase() = default;

    void Serialize( void* data, size_t size );
    virtual void Serialize( u64& data ) = 0;
    virtual void Serialize( s64& data ) = 0;
    virtual void Serialize( f32& data ) = 0;
    virtual void Serialize( f64& data ) = 0;
    virtual void Serialize( bool& data ) = 0;
    virtual void Serialize( u32& data ) = 0;
    virtual void Serialize( s32& data ) = 0;
    virtual void Serialize( u16& data ) = 0;
    virtual void Serialize( s16& data ) = 0;
    virtual void Serialize( u8& data ) = 0;
    virtual void Serialize( s8& data ) = 0;

    template<typename T>
    IOStreamBase& operator<<( T& data )
    {
        Serialize( data );
        return *this;
    }


protected:
    ArchiveBase& _archive;

};

} // LXX

#endif //XLUA_IOSTREAMBASE_H
