//
// Created by Harry on 9/2/2023.
//

#ifndef XLUA_BYTECODECHUNK_H
#define XLUA_BYTECODECHUNK_H
#include <core/objects/GCObject.h>
#include <core/Containers/Array.h>
#include <core/Containers/UnorderedMap.h>
#include <core/vm/Instructions/InstructionSet.h>

namespace LXX
{

class State;
class VirtualMachine;
class Value;
class LuaClosure;
template< typename CharType >
class TString;
typedef TString< char > String;

using namespace Instruction;

class ByteCodeChunk : public GCObject
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    ByteCodeChunk( Array< String* > parameters );

    u32 AddConstValue( s32 value );
    u32 AddConstValue( f64 value );
    u32 AddConstValue( bool value );
    u32 AddConstValue( const char* value );
    u32 AddConstValue( String* value );
    u32 AddConstValue( LuaClosure* value );
    u32 GetConstNilValueIndex() { return 0; }

    Array< u64 >& GetCode() { return _code; }
    const Array< u64 >& GetCode() const { return _code; }

    bool ConstValueIdxIsValid( u32 idx ) const { return idx < _consts.Size(); }

    const Value* GetConstValue( u32 idx ) const ;
    Value* GetConstValue( u32 idx );

    bool AddLocalVariable(String *name , u32 *outIdx = nullptr);
    bool IsLocalVariable( String *name , u32 *outIdx = nullptr);
    bool IsParameterVariable(String *name , u32 *outIdx = nullptr);
    u32 AddTempVariable();
    u32 GetLastTempVariableIndex() { return _lastTempVariableIndex; };
    u32 ResetLastTempVariableIndex() { return _lastTempVariableIndex = -1 ; };

    u32 GetLocalVariableCount() const { return _localVariables.GetSize(); }
    u32 GetTemporaryVariableCount() const { return _maxTempVariableNum ; }
    /*
     * Get the number of parameters exclude the variable arguments
     * */
    u32 GetDeclareArgumentVariableNum() const;
private:
    bool VariableIsIn( Array< String* > &variables ,  String *name ,  u32 *outIdx = nullptr );
private:

    Array< u64 > _code;
    mutable Array< Value > _consts;
    Array< String* > _parameters;
    Array< String* > _localVariables;
    u32 _lastTempVariableIndex = 0;
    u32 _maxTempVariableNum = 0;
};

} // LXX

#endif //XLUA_BYTECODECHUNK_H
