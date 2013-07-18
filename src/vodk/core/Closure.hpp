// Copyright (c) 2013 Nicolas Silva
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

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
