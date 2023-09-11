//
// Created by Harry on 8/4/2023.
//

#ifndef XLUA_VM_H
#define XLUA_VM_H
#include <Core/Value.h>
#include <Core/State.h>
#include <Core/GlobalState.h>
#include <core/compiler/Compiler.h>
#include <Core/vm/Instructions/InstructionSet.h>
#include <Core/vm/Instructions/InstructionDecoder.h>
#include <core/exceptions/VirtualMachineExecuteException.h>
#include <core/containers/UnorderedMap.h>

namespace LXX
{

using namespace Instruction;

struct VMSystemFunctionKey
{
    u32 _systemFunctionID;
    u32 _systemCallArgument1;
    u32 _systemCallArgument2;
    VMSystemFunctionKey( u32 systemFunctionID
            , u32 systemCallArgument1
            , u32 systemCallArgument2
        )
        : _systemFunctionID( systemFunctionID )
        , _systemCallArgument1( systemCallArgument1 )
        , _systemCallArgument2( systemCallArgument2 )
    {

    }
    VMSystemFunctionKey( )
        : _systemFunctionID( -1 )
        , _systemCallArgument1( -1 )
        , _systemCallArgument2( -1 )
    {

    }

    template< typename SystemFunctionID,
            typename SystemCallArgument1,
            typename SystemCallArgument2
            >
    VMSystemFunctionKey( SystemFunctionID systemFunctionID
        ,  SystemCallArgument1 systemCallArgument1
        ,   SystemCallArgument2 systemCallArgument2
        )
        : VMSystemFunctionKey( static_cast<u32>(systemFunctionID),
                               static_cast<u32>(systemCallArgument1),
                               static_cast<u32>(systemCallArgument2) )
    {

    }


    VMSystemFunctionKey( const VMSystemFunctionKey &other ) = default;
    VMSystemFunctionKey( VMSystemFunctionKey &&other ) = default;
    VMSystemFunctionKey &operator=( const VMSystemFunctionKey &other ) = default;

    bool  operator==( const VMSystemFunctionKey &other )
    {
        return _systemFunctionID == other._systemFunctionID
                && _systemCallArgument1 == other._systemCallArgument1
                && _systemCallArgument2 == other._systemCallArgument2;
    }

    bool  operator!=( const VMSystemFunctionKey &other )
    {
        return !( *this == other );
    }


    u32 Hash()
    {
        return ( _systemFunctionID & 0xffff ) << 16
            | ( _systemCallArgument1 & 0xff ) << 8
            | ( _systemCallArgument2 & 0xff );
    }
};

template<>
u32 Hash( VMSystemFunctionKey key );

class VMSystemCallBindingBase : public GCObject
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    VMSystemCallBindingBase() = default;
    virtual ~VMSystemCallBindingBase() = default;
    virtual void Invoke() = 0;
};

class VirtualMachine;

template<typename SystemFunctionID,
        typename SystemCallArgument1>
class VMSystemCallBinding : public VMSystemCallBindingBase
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    typedef void (VirtualMachine::*OnSystemCall)( SystemCallArgument1 );
    VMSystemCallBinding( VirtualMachine *vm
                         , OnSystemCall onSystemCall
                         , SystemCallArgument1 systemCallArgument1
                         )
    : _vm( vm )
    , _onSystemCall( onSystemCall )
    , _systemCallArgument1( systemCallArgument1 )
    {

    }
    virtual void Invoke()
    {
        (_vm->*_onSystemCall)( _systemCallArgument1 );
    }
private:
    VirtualMachine *_vm;
    OnSystemCall _onSystemCall;
    SystemCallArgument1 _systemCallArgument1;
};


class VirtualMachine
{
    friend class ByteCodeChunk;
public:
    VirtualMachine();
    bool Startup();
    void Shutdown();

    bool ProtectCall(State *state, u32 nArgs );


    template< typename FunctionType, typename ...Args>
    bool Invoke( FunctionType func , Args ...args )
    {
        State *state = _globalState->NewState();
        state->GetStack().PushFunction<FunctionType>(  func );
        u32 oldTop = state->GetStack().GetTop();
        state->GetStack().Push( args ... );
        u32 newTop = state->GetStack().GetTop();

        u32 ArgumentNum = newTop - oldTop;

        if( !ProtectCall(state, ArgumentNum) )
            return false;

        state->GetStack().Pop( state->GetLastFunctionCallReturnValueCount() );

        return true;
    }

    template< typename FunctionType, typename resultType , typename ...Args>
    bool InvokeWithResult( FunctionType func , resultType &result , Args ...args )
    {
        State *state = _globalState->NewState();

        state->GetStack().PushFunction<FunctionType>(  func );
        u32 oldTop = state->GetStack().GetTop();
        state->GetStack().Push( args ... );
        u32 newTop = state->GetStack().GetTop();
        u32 ArgumentNum = newTop - oldTop;

        if( !ProtectCall(state, ArgumentNum) ) return false;

        result = state->GetStack().As< resultType >( -1 );

        state->GetStack().Pop( state->GetLastFunctionCallReturnValueCount() );

        return true;
    }

    GlobalState *GetGlobalState() { return _globalState; }

    LuaClosure* CompileFile( String *scriptFileName );
    LuaClosure* CompileString( const char* scriptContent );
private:
    u32 CallLuaClosure( State *state , LuaClosure *closure );
private:
    void OnStartup();
    void OnShutdown();
    template<typename ...Args>
    void ThrowError( const char *format, Args ...args )
    {
        throw VirtualMachineExecuteException( format , std::forward<Args>(args) ... );
    }

private:
    // >>>  InstructionExecuteFunction
    struct InstructionExecuteContext
    {
        InstructionExecuteContext( VirtualMachine* vm , State* state , ByteCodeChunk* chunk )
            : _vm( vm )
            , _state( state )
            , _chunk( chunk )
        {

        }
        VirtualMachine* _vm;
        State* _state;
        ByteCodeChunk* _chunk;
        Opcode _opcode;
        Value* _destOperand;
        Value* _srcOperand1;
        Value* _srcOperand2;
    };
    void Decode( u64 code , InstructionExecuteContext &context );

    typedef void ( VirtualMachine::*InstructionExecuteFunction )( InstructionExecuteContext &context );

    void RegisterInstructionSet();
    // >>> InstructionExecute_XXX
    void InstructionExecute_OpcodeCall( InstructionExecuteContext &context );
    void InstructionExecute_OpcodeReturn( InstructionExecuteContext &context );
    void InstructionExecute_OpcodeJump( InstructionExecuteContext &context );
    void InstructionExecute_OpcodeSystemCall( InstructionExecuteContext &context );
    void InstructionExecute_OpcodeAssert( InstructionExecuteContext &context );
    void InstructionExecute_OpcodePush( InstructionExecuteContext &context );
    void InstructionExecute_OpcodePop( InstructionExecuteContext &context );
    void InstructionExecute_OpcodeNewTable( InstructionExecuteContext &context );
    void InstructionExecute_OpcodeGetField( InstructionExecuteContext &context );
    void InstructionExecute_OpcodeSetField( InstructionExecuteContext &context );
    void InstructionExecute_OpcodeGetVariableArgument( InstructionExecuteContext &context );
    void InstructionExecute_Assignment( InstructionExecuteContext &context );
    void InstructionExecute_OpcodeBinaryOpPower( InstructionExecuteContext &context );
    void InstructionExecute_OpcodeBinaryOpCmpValueType( InstructionExecuteContext &context );
    void InstructionExecute_OpcodeBinaryOpCmpLessThan( InstructionExecuteContext &context );
    void InstructionExecute_OpcodeBinaryOpCmpLessThanOrEqual( InstructionExecuteContext &context );
    void InstructionExecute_OpcodeBinaryOpCmpEqual( InstructionExecuteContext &context );
    void InstructionExecute_OpcodeBinaryOpCmpNotEqual( InstructionExecuteContext &context );
    void InstructionExecute_OpcodeBinaryOpCmpGreaterThan(InstructionExecuteContext &context );
    void InstructionExecute_OpcodeBinaryOpCmpGreaterThanOrEqual(InstructionExecuteContext &context );
    void InstructionExecute_OpcodeBinaryOpAdd( InstructionExecuteContext &context );
    void InstructionExecute_OpcodeBinaryOpSubtract( InstructionExecuteContext &context );
    void InstructionExecute_OpcodeBinaryOpMultiply( InstructionExecuteContext &context );
    void InstructionExecute_OpcodeBinaryOpDivide( InstructionExecuteContext &context );
    void InstructionExecute_OpcodeBinaryOpIntDivide( InstructionExecuteContext &context );
    void InstructionExecute_OpcodeBinaryOpModulo( InstructionExecuteContext &context );
    void InstructionExecute_OpcodeBinaryOpBitwiseAnd( InstructionExecuteContext &context );
    void InstructionExecute_OpcodeBinaryOpBitwiseOr( InstructionExecuteContext &context );
    void InstructionExecute_OpcodeBinaryOpBitwiseXor( InstructionExecuteContext &context );
    void InstructionExecute_OpcodeBinaryOpShiftLeft( InstructionExecuteContext &context );
    void InstructionExecute_OpcodeBinaryOpShiftRight( InstructionExecuteContext &context );
    void InstructionExecute_OpcodeBinaryOpConcat( InstructionExecuteContext &context );
    void InstructionExecute_OpcodeBinaryOpLogicalAnd( InstructionExecuteContext &context );
    void InstructionExecute_OpcodeBinaryOpLogicalOr( InstructionExecuteContext &context );
    void InstructionExecute_OpcodeUnaryMinus( InstructionExecuteContext &context );
    void InstructionExecute_OpcodeUnaryBitwiseNot( InstructionExecuteContext &context );
    void InstructionExecute_OpcodeUnaryNot( InstructionExecuteContext &context );
    void InstructionExecute_OpcodeUnaryLen( InstructionExecuteContext &context );
    void InstructionExecute_MetaMethodBinaryOpCall(InstructionExecuteContext &context , const char * methodKey );
    void InstructionExecute_MetaMethodUnaryOpCall(InstructionExecuteContext &context , const char * methodKey );
    // <<< InstructionExecute_XXX
    // <<<  InstructionExecuteFunction

    // >>> SystemFunction
#define BEGIN_REGISTER_SYSTEM_FUNCTION() _systemFunctionBindings = {
#define END_REGISTER_SYSTEM_FUNCTION() };
#define REGISTER_SYSTEM_FUNCTION_ITEM( systemFunction , SystemArgumentScope , SystemArgument ) \
        {                                                                                      \
                VMSystemFunctionKey( SystemFunction::systemFunction , SystemArgumentScope::SystemArgument , 0 ) , \
                    new VMSystemCallBinding< SystemFunction, SystemArgumentScope>( this ,\
                       &VirtualMachine::OnSystemFunction_##systemFunction##_##SystemArgument \
                    , SystemException::SystemArgument \
                ) \
        },
#define DECLARE_SYSTEM_FUNCTION( systemFunction , SystemArgument ) \
        void OnSystemFunction_##systemFunction##_##SystemArgument( SystemException systemException );
#define IMPLEMENTATION_SYSTEM_FUNCTION( systemFunction , SystemArgument ) \
        void VirtualMachine::OnSystemFunction_##systemFunction##_##SystemArgument( SystemException systemException )

    void OnSystemFunction( u32 systemFunction , u32 argument1 , u32 argument2 );
    void RegisterSystemFunction();
    DECLARE_SYSTEM_FUNCTION( RaiseException , ExceptionForZeroReturnValueCount );
    DECLARE_SYSTEM_FUNCTION( RaiseException , ExceptionForStatementLimitExpressionIsNotNumber );
    DECLARE_SYSTEM_FUNCTION( RaiseException , ExceptionForStatementIncreaseStepIsNotNumberAndIsNotNil );
    DECLARE_SYSTEM_FUNCTION( RaiseException , ExceptionForStatementIncreaseStepIsEqualZero );
    // <<< SystemFunction

    Value* FindImmediateValue( s32 valueId );
private:
    Array< InstructionExecuteFunction > _instructionExecuteFunctions;
    // todo , hold compiler to prevent gc
    Array< Compiler * >  _compilers;
    UnorderedMap< s32 , Value > _immediateValues;

    // system function
    UnorderedMap< VMSystemFunctionKey , VMSystemCallBindingBase* > _systemFunctionBindings;
private:
    GlobalState *_globalState;
};

}

#endif //XLUA_VM_H