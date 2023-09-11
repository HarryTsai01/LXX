//
// Created by Harry on 8/5/2023.
//

#ifndef XLUA_VIRTUALMACHINEEXECUTEEXCEPTION_H
#define XLUA_VIRTUALMACHINEEXECUTEEXCEPTION_H
#include "ExceptionBase.h"

namespace LXX
{

class VirtualMachineExecuteException : public  ExceptionBase
{
public:
    VirtualMachineExecuteException( const char *message );
    template <typename ... Args>
    VirtualMachineExecuteException( const char *format, Args ... args )
    {
        _message = String::Format( format, std::forward<Args>( args )... );
    }

    virtual String* ToString() const override;
private:
    String *_message;
};

} // LXX

#endif //XLUA_VIRTUALMACHINEEXECUTEEXCEPTION_H
