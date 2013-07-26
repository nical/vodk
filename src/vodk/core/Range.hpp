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

#ifndef KIWI_UTILS_RANGE_HPP
#define KIWI_UTILS_RANGE_HPP

#include <stdint.h>
#include <assert.h>
#include "vodk/core/Range.hpp"

/**
 * Typed view on a bounded buffer of memory.
 * Range classes don't own the data they point to.
 */
template<typename T>
class Range {
public:
    typedef T ValueType;
    typedef Range<T> Self;
    typedef T* iterator;
    typedef const T* const_iterator;

    Range(T* s, T* e) : _start(s), _end(e) {}
    Range(T* aStart, uint32_t aSize) : _start(aStart), _end(aStart+aSize) {}

    T& operator[](uint32_t i) {
        assert(i < size());
        return _start[i];
    }

    const T& operator[](uint32_t i) const {
        assert(i < size());
        return _start[i];
    }

    /**
     * Returns a slice of this range.
     */
    Self operator()(int32_t s, int32_t e){
        return Self(_start+s, _end+e);
    }

    /**
     * Returns a pointer to the first element.
     */
    T* pointer() {
        return _start;
    }

    const T* pointer() const {
        return _start;
    }

    bool operator==(Range rhs) {
        return _start == rhs._start && _end == rhs._end;
    }

    /**
     * Returns true if s is withing this range.
     */
    bool contains(Range s) const {
        return _start <= s._start && _end >= s._end;
    }

    /**
     * returns true if this is not an empty range.
     */
    bool is_valid() const {
        return _start <= _end && _start != nullptr;
    }

    void shrink_left(uint32_t num) {
        _start += num;
        assert (_start <= _end);
    }

    void shrink_right(uint32_t num) {
        _end -= num;
        assert (_start <= _end);
    }

    Range<uint8_t> bytes() const {
        return Range<uint8_t>(reinterpret_cast<uint8_t*>(_start), (_end - _start) * sizeof(_start) );
    }

    Self next() const {
        Self s = *this;
        s.shrink_left();
        return s;
    }

    bool has_next() const {
        return !next().empty();
    }

    // stl style interface

    int32_t size() const {
        return _end - _start;
    }

    int32_t empty() const {
        return size() == 0;
    }

    iterator begin() const {
        return _start;
    }

    iterator end() const {
        return _end;
    }

    T& pop_front() {
        assert(_start < _end);
        return *_start++;
    }

    T& pop_back() {
        assert(_start < _end);
        return *--_end;
    }

protected:
    T* _start;
    T* _end;
friend class DynamicRange;
};

template<typename T> Range<T> range(T* ptr, uint32_t size) {
    return Range<T>(ptr,size);
}

typedef Range<uint8_t> ByteRange;

/**
 * A Range class for use when the size of the elements is not known at
 * compile-time.
 */
class DynamicRange
{
public:
    typedef DynamicRange Self;

    void* operator[](uint32_t i) {
        return &_range[i*_step];
    }

    DynamicRange(void* start, void* end, uint8_t step)
    : _range(ByteRange((uint8_t*)start, (uint8_t*)end))
    , _step(step)
    {}

    template<typename T>
    DynamicRange(Range<T> r)
    : _range(r.bytes())
    , _step(sizeof(T))
    {}

    template<typename T>
    T* get(uint32_t idx) {
        assert(sizeof(T) == _step);
        return static_cast<T*>((*this)[idx]);
    } 

    Self operator()(int32_t s, int32_t e) {
        return Self(_range._start + s*_step,
                    _range._end + e*_step,
                    _step);
    }

    void* pointer() {
        return _range.pointer();
    }

    bool operator==(Self rhs) {
        return _step == rhs._step && _range == rhs._range;
    }

    bool contains(Self s) {
        return _range.contains(s._range);
    }

    bool is_valid() {
        return _range.is_valid();
    }

    void shrink_left(uint32_t num) {
        _range.shrink_left(num*_step);
    }

    void shrink_right(uint32_t num) {
        _range.shrink_right(num*_step);
    }

    Range<uint8_t> bytes() {
        return _range;
    }

    Self next() const {
        Self s = *this;
        s.shrink_left(1);
        return s;
    }

    bool is_valid() const {
        return _range.is_valid();
    }

    bool has_next() const {
        return !next().empty();
    }

    uint8_t step() const {
        return _step;
    }

    // stl style interface

    int32_t size() const {
        return _range.size()/_step;
    }

    int32_t empty() const {
        return size() == 0;
    }

    void* pop_front() {
        void* start = _range._start;
        shrink_left(1);
        return start;
    }

    void* pop_back() {
        void* end = _range._end;
        shrink_right(1);
        return end;
    }
private:
    ByteRange _range;
    uint8_t _step; // in bytes
};

#endif
