//
// Created by Harry on 8/5/2023.
//

#ifndef XLUA_VIRTUALMACHINEEXECUTEEXCEPTION_H
#define XLUA_VIRTUALMACHINEEXECUTEEXCEPTION_H
#include "ExceptionBase.h"

namespace LXX
{


class State;
class VirtualMachineExecuteException : public  ExceptionBase
{
public:
    VirtualMachineExecuteException(  State *state , String* message );
    VirtualMachineExecuteException(  State *state , const char *message )
     : VirtualMachineExecuteException( state , NEW_STRING(message) )
    {

    }
    template <typename ... Args>
    VirtualMachineExecuteException( State *state , const char *format, Args ... args )
    : VirtualMachineExecuteException( state  , String::Format( format, std::forward<Args>( args )... ) )
    {
    }

    virtual String* ToString() const override;
private:
    String *_message;
    String *_backtrace;
    State *_state;
};

} // LXX

#endif //XLUA_VIRTUALMACHINEEXECUTEEXCEPTION_H
