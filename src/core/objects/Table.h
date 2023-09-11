//
// Created by Harry on 9/3/2023.
//

#ifndef XLUA_TABLE_H
#define XLUA_TABLE_H
#include <core/objects/GCObject.h>
#include <core/containers/UnorderedMap.h>
#include <core/Value.h>

namespace LXX
{

class VirtualMachine;

class Table : public GCObject
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    Table() = default;

    Value* GetField( VirtualMachine* vm, Value key );
    void SetField(  VirtualMachine* vm, Value key, Value value );

    Value* RawGetField( Value key );
    void RawSetField( Value key, Value value );

    Table* GetMetaTable();
    void SetMetaTable( Table* table );
private:
    template<typename ...Args>
    void ThrowError( const char *format, Args ...args )
    {
        throw VirtualMachineExecuteException( format , std::forward<Args>(args) ... );
    }
private:
    UnorderedMap<Value, Value> _data;
    constexpr static const char * META_TABLE_NAME = "__metatable";
    constexpr static const char * META_METHOD_INDEX_NAME = "__index";
    constexpr static const char * META_METHOD_NEW_INDEX_NAME = "__newindex";
};

} // LXX

#endif //XLUA_TABLE_H
