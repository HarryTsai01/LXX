//
// Created by Harry on 9/14/2023.
//

#ifndef XLUA_UTILITIES_H
#define XLUA_UTILITIES_H

namespace LXX
{


template<typename T>
T Max(T a, T b)
{
    return a > b ? a : b;
}


template<typename T>
T Min(T a, T b)
{
    return a < b ? a : b;
}


template<typename T>
T clamp(T value, T min, T max )
{
    return Min(Max(value, min), max);
}

}

#endif //XLUA_UTILITIES_H
