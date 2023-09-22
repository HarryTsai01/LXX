//
// Created by Harry on 9/11/2023.
//

#ifndef XLUA_METAMETHODHANDLER_H
#define XLUA_METAMETHODHANDLER_H
#include <core/objects/GCObject.h>
#include <core/containers/UnorderedMap.h>
#include <core/Value.h>
#include <core/exceptions/VirtualMachineExecuteException.h>

namespace LXX
{

class State;

class MetaMethodHandler : public GCObject
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    typedef void (MetaMethodHandler::*MetaMethod)( State * state, Value * destOperand , Value * srcOperand1 , Value * srcOperand2 );

    bool Invoke( State * state, const char * metaMethodKey , Value * destOperand , Value * srcOperand1 , Value * srcOperand2 );

    constexpr static const char * META_TABLE_NAME = "__metatable";
    constexpr static const char * META_METHOD_KEY_BIN_OP_ADD = "__add";
    constexpr static const char * META_METHOD_KEY_BIN_OP_SUB = "__sub";
    constexpr static const char * META_METHOD_KEY_BIN_OP_MUL = "__mul";
    constexpr static const char * META_METHOD_KEY_BIN_OP_DIV = "__div";
    constexpr static const char * META_METHOD_KEY_BIN_OP_MOD = "__mod";
    constexpr static const char * META_METHOD_KEY_BIN_OP_POW = "__pow";
    constexpr static const char * META_METHOD_KEY_UNA_OP_MINUS = "__unm";
    constexpr static const char * META_METHOD_KEY_BIN_OP_IDIV = "__idiv";
    constexpr static const char * META_METHOD_KEY_BIN_OP_BITWISE_AND = "__band";
    constexpr static const char * META_METHOD_KEY_BIN_OP_BITWISE_OR = "__bor";
    constexpr static const char * META_METHOD_KEY_BIN_OP_BITWISE_XOR = "__bxor";
    constexpr static const char * META_METHOD_KEY_UNA_OP_BITWISE_NOT = "__bnot";
    constexpr static const char * META_METHOD_KEY_BIN_OP_SHIFT_LEFT = "__shl";
    constexpr static const char * META_METHOD_KEY_BIN_OP_SHIFT_RIGHT = "__shr";
    constexpr static const char * META_METHOD_KEY_BIN_OP_CONCAT  = "__concat";
    constexpr static const char * META_METHOD_KEY_UNA_OP_LEN = "__len";
    constexpr static const char * META_METHOD_KEY_BIN_OP_EQUAL = "__eq";
    constexpr static const char * META_METHOD_KEY_BIN_OP_LESS_THAN = "__lt";
    constexpr static const char * META_METHOD_KEY_BIN_OP_LESS_THAN_OR_EQUAL = "__le";
    constexpr static const char * META_METHOD_KEY_INDEX = "__index";
    constexpr static const char * META_METHOD_KEY_NEW_INDEX = "__newindex";
    constexpr static const char * META_METHOD_KEY_CALL = "__call";

protected:
    template<typename ...Args>
    void ThrowError( State *state , const char *format, Args ...args )
    {
        throw VirtualMachineExecuteException( state , format , std::forward<Args>(args) ... );
    }

    UnorderedMap< const char* , MetaMethod> _metaMethodMap;
};

#define META_METHOD_BEGIN_REGISTER() _metaMethodMap = {
#define META_METHOD_REGISTER(ClassName , metaMethodKey) { metaMethodKey , static_cast<MetaMethodHandler::MetaMethod>(&ClassName::MetaMethod##metaMethodKey) },
#define META_METHOD_END_REGISTER() };
#define DECLARE_META_METHOD( metaMethodKey ) void MetaMethod##metaMethodKey( State *state , Value * destOperand , Value * srcOperand1 , Value * srcOperand2 );
#define IMPLEMENT_META_METHOD( className , metaMethodKey ) void className::MetaMethod##metaMethodKey( State *state , Value * destOperand , Value * srcOperand1 , Value * srcOperand2 )

} // LXX

#endif //XLUA_METAMETHODHANDLER_H
