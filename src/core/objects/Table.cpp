//
// Created by Harry on 9/3/2023.
//

#include "Table.h"
#include <core/exceptions/VirtualMachineExecuteException.h>
#include <core/vm/VirtualMachine.h>
#include <core/objects/metatable/MetaMethodHandler.h>

namespace LXX
{

Value* Table::GetField( VirtualMachine* vm , Value key )
{
    Value* value = RawGetField( key );
    if( value != nullptr )
        return value;
    Table* metaTable = GetMetaTable();
    if( metaTable != nullptr )
    {
        Value* metaMethod = metaTable->RawGetField( MetaMethodHandler::META_METHOD_KEY_INDEX );
        if( metaMethod != nullptr)
        {
            if( metaMethod->IsFunction() )
            {
               if( metaMethod->IsLightCFunction() )
               {
                   CFunction function = metaMethod->AsLightCFunction();
                   Value* result = nullptr;
                   if( vm->InvokeWithResult( function, result, this, &key ) )
                   {
                        return result;
                   }
               }
               else if( metaMethod->IsLuaClosure() )
               {
                   LuaClosure *function = metaMethod->AsLuaClosure();
                   Value* result = nullptr;
                   if( vm->InvokeWithResult( function, result, this, &key ) )
                   {
                       return result;
                   }
               }
            }
            else if( metaMethod->IsTable() )
            {
                return metaMethod->AsTable()->GetField( vm , key );
            }
        }
    }
    return nullptr;
}


void Table::SetField( VirtualMachine* vm ,Value key, Value value )
{
    // check if new index meta method exists
    Table* metaTable = GetMetaTable();
    if( metaTable != nullptr )
    {
        Value* metaMethod = metaTable->RawGetField( MetaMethodHandler::META_METHOD_KEY_NEW_INDEX );
        if( metaMethod != nullptr )
        {
            if( metaMethod->IsFunction() )
            {
                if( metaMethod->IsLightCFunction() )
                {
                    vm->Invoke( metaMethod->AsLightCFunction(), this, &key, &value );
                    return;
                }
                else if( metaMethod->IsLuaClosure() )
                {
                    vm->Invoke( metaMethod->AsLuaClosure(), this, &key, &value );
                    return;
                }
            }
            else if( metaMethod->IsTable() )
            {
                metaMethod->AsTable()->SetField( vm , key, value );
                return;
            }
        }
    }

    RawSetField( key, value );
}


Value* Table::RawGetField( Value key )
{
    auto It = _data.Find( key );
    if( It != _data.End() )
        return &It->Second;
    return nullptr;
}


void Table::RawSetField( Value key, Value value )
{
    _data.Add( key, value );
}


Table* Table::GetMetaTable()
{
    Value* metaTable = RawGetField( MetaMethodHandler::META_TABLE_NAME );
    if( metaTable != nullptr
    && metaTable->IsTable() )
    {
        return metaTable->AsTable();
    }
    return nullptr;
}


void Table::SetMetaTable( Table* table )
{
    RawSetField(MetaMethodHandler::META_TABLE_NAME, Value(table) );
}


String* Table::ToString() const
{
    return String::Format("table(%p)",this);
}

} // LXX