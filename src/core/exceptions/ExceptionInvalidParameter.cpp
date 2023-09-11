//
// Created by Harry on 8/5/2023.
//

#include "ExceptionInvalidParameter.h"

namespace LXX
{


ExceptionInvalidParameter::ExceptionInvalidParameter( s32 idx , const char *message)
: Index( idx)
, Message( NEW_STRING( message ) )
{
}



} // LXX