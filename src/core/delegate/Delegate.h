//
// Created by Harry on 9/16/2023.
//

#ifndef XLUA_DELEGATE_H
#define XLUA_DELEGATE_H
#include <utility>
#include <core/LXX.h>
#include <core/mem/MemoryAllocator.h>
#include <cassert>
#include <core/objects/GCObject.h>

namespace LXX
{

template<typename ReturnType, typename ...Args>
class DelegateBindings : public GCObject
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    DelegateBindings() = default;

    virtual ~DelegateBindings() = default;

    virtual ReturnType Invoke(Args... args) = 0;

    ReturnType operator()(Args... args) {
        return Invoke(std::forward<Args>(args)...);
    }
};

template<typename ClassName, typename ClassMethod, typename ReturnType, typename ...Args>
class ClassMemberDelegateBindings : public DelegateBindings<ReturnType, Args...>
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    ClassMemberDelegateBindings(ClassName *classInst, ClassMethod classMethod)
            : _classInst(classInst), _method(classMethod) {

    }

    virtual ~ClassMemberDelegateBindings() = default;

    virtual ReturnType Invoke(Args... args) {
        return (_classInst->*_method)(std::forward<Args>(args)...);
    }

private:
    ClassName *_classInst;
    ClassMethod _method;
};

template<typename StaticMethod, typename ReturnType, typename ...Args>
class StaticDelegateBindings : public DelegateBindings<ReturnType, Args...>
{
    OPERATOR_NEW_DELETE_OVERRIDE_ALL
public:
    StaticDelegateBindings(StaticMethod staticMethod)
            : _method(staticMethod) {

    }

    virtual ~StaticDelegateBindings() = default;

    virtual ReturnType Invoke(Args... args) {
        return (_method)(std::forward<Args>(args)...);
    }

private:
    StaticMethod _method;
};


template<typename ReturnType, typename ...Args>
class Delegate
{
public:
    Delegate()
     : _bindings( nullptr )
    {
    }

    Delegate(const Delegate & other)
            : _bindings( other._bindings )
    {
    }

    Delegate(Delegate && other)
    :_bindings(other._bindings)
    {
        other._bindings = nullptr;
    }

    virtual ~Delegate()
    {
        _bindings = nullptr;
    }

    Delegate &operator=(const Delegate & other)
    {
        _bindings = other._bindings;
        return *this;
    }

    Delegate &operator=(Delegate && other )
    {
        _bindings = other._bindings;
        other._bindings = nullptr;
        return *this;
    }


    ReturnType InvokeIfBound(Args... args)
    {
        if( !IsBound() ) return ReturnType();
        Invoke( std::forward<Args>(args)... );
    }
    ReturnType Invoke(Args... args)
    {
        assert( IsBound() );
        if (_bindings != nullptr)
            return _bindings->Invoke(std::forward<Args>(args)...);
        return ReturnType();
    }

    ReturnType operator()(Args... args) {
        return Invoke(std::forward<Args>(args)...);
    }

    bool IsBound() const { return _bindings != nullptr; }

    template<typename ClassName, typename ClassMethod>
    void Bind(ClassName *classInst, ClassMethod classMethod) {
        assert(!IsBound());
        _bindings = new ClassMemberDelegateBindings<ClassName, ClassMethod, ReturnType, Args...>(classInst,
                                                                                                 classMethod);
    }

    void Unbind()
    {
        _bindings = nullptr;
    }

    template<typename StaticMethod>
    void Bind(StaticMethod staticMethod) {
        assert(!IsBound());
        _bindings = new StaticDelegateBindings<StaticMethod, ReturnType, Args...>(staticMethod);
    }


private:
    DelegateBindings<ReturnType, Args...> *_bindings;
};

#define DEFINE_DELEGATE(DelegateName, ReturnType) \
    class Delegate##DelegateName : public Delegate<ReturnType> \
    {                                                  \
    };

#define DEFINE_DELEGATE_WITH_PARAMS(DelegateName, ReturnType, ...) \
    class Delegate##DelegateName : public Delegate<ReturnType, __VA_ARGS__> \
    {                                                  \
    };

#define DEFINE_EVENT(EventName) \
    class Event##EventName : public Delegate<void> \
    {                                                  \
    };

#define DEFINE_EVENT_WITH_PARAMS(EventName, ...) \
    class Event##EventName : public Delegate<void, __VA_ARGS__> \
    {                                                  \
    };
#define DEFINE_CLASS_EVENT( EventName ) \
    public:                             \
    template<typename Method>         \
    void Bind##EventName( Method method )\
    { \
        _##EventName.Bind( method ); \
    }                                   \
    template<typename ClassName, typename Method> \
    void Bind##EventName( ClassName *classInst, Method method )\
    { \
        _##EventName.Bind( classInst, method ); \
    }                                   \
    private:                            \
    void Invoke##EventName()           \
    { \
        _##EventName(); \
    }                               \
    Event##EventName _##EventName;
}
#endif //XLUA_DELEGATE_H
