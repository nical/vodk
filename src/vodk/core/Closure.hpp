
#ifndef VODK_CORE_CLOSURE_HPP
#define VODK_CORE_CLOSURE_HPP

namespace vodk {
namespace core {

template<class T>
struct Closure {
    virtual void call(T& param) = 0;
    virtual ~Closure() {}
};

template<class T>
class FunctionClosure : public Closure<T> {
public:
    typedef void(*FuncPtr)(T&);
    FunctionClosure(FuncPtr callback)
    : _function(callback) {}

    void call(T& params) {
        _function(params);
    }
private:
    FuncPtr _function;
};

template<class T>
FunctionClosure<T>*
closure( typename FunctionClosure<T>::FuncPtr f ) {
    return new FunctionClosure<T>(f);
}

} // core
} // vodk

#endif
