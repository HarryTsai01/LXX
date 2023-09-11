//
// Created by Harry on 9/2/2023.
//

#ifndef XLUA_LUACLOSURE_H
#define XLUA_LUACLOSURE_H
#include <core/objects/GCObject.h>
#include <core/Containers/Array.h>
#include <core/vm/ByteCodeChunk.h>
#include <core/objects/string/String.h>

namespace LXX
{


class Stack;
class VirtualMachine;
class LuaClosure : public GCObject
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    LuaClosure( ByteCodeChunk* chunk );

    const ByteCodeChunk* GetChunk() const { return _chunk; }
    ByteCodeChunk* GetChunk() { return _chunk; }

    void AddLuaClosure( LuaClosure* closure ) ;

    void SetName( const char* name ) { _name = NEW_STRING( name ); }
    String *GetName() const { return _name; }

    void SetParameterCount( u32 count ) { _parameterCount = count; }
    u32 GetParameterCount() const { return _parameterCount; }

    u32 GetLocalVariableCount() const { return _chunk->GetLocalVariableCount(); }
    u32 GetTemporaryVariableCount() const { return _chunk->GetTemporaryVariableCount(); }
private:
    String *_name;
    /*
     *   0 : no parameter
     *  -1 : unknown parameter count
     *   n : n parameters
     * */
    u32 _parameterCount;
    ByteCodeChunk* _chunk;
    LuaClosure* _parent;
    Array< LuaClosure* > _children;
};

} // LXX

#endif //XLUA_LUACLOSURE_H
