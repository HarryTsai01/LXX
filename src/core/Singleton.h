//
// Created by Harry on 9/16/2023.
//

#ifndef XLUA_SINGLETON_H
#define XLUA_SINGLETON_H
namespace LXX
{
template<typename T>
class Singleton
{
public:
    static T& GetInstance()
    {
        if (_sInst == nullptr)  _sInst = new T();
        return *_sInst;
    }
protected:
    static T *_sInst;
};

template<typename T>
T* Singleton<T>::_sInst = nullptr;

}

#endif //XLUA_SINGLETON_H
