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
    void Serialize( const void* data, size_t size );
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
    virtual void Serialize( const u64& data ) = 0;
    virtual void Serialize( const s64& data ) = 0;
    virtual void Serialize( const f32& data ) = 0;
    virtual void Serialize( const f64& data ) = 0;
    virtual void Serialize( const bool& data ) = 0;
    virtual void Serialize( const u32& data ) = 0;
    virtual void Serialize( const s32& data ) = 0;
    virtual void Serialize( const u16& data ) = 0;
    virtual void Serialize( const s16& data ) = 0;
    virtual void Serialize( const u8& data ) = 0;
    virtual void Serialize( const s8& data ) = 0;

    template<typename T>
    IOStreamBase& operator<<( T& data )
    {
        Serialize( data );
        return *this;
    }

    template<typename T>
    IOStreamBase& operator<<( const T& data )
    {
        Serialize( data );
        return *this;
    }

    IOStreamBase& operator<<( const char* data )
    {
        Serialize( data , std::strlen(data) );
        return *this;
    }


    template<typename T>
    IOStreamBase& operator>>( T& data )
    {
        Serialize( data );
        return *this;
    }

    bool IsReading() const { return _archive.IsReader() ; }
    bool IsWriting() const { return _archive.IsWriter() ; }


protected:
    ArchiveBase& _archive;

};


} // LXX

#endif //XLUA_IOSTREAMBASE_H
