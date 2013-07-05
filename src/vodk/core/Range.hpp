#ifndef KIWI_UTILS_SLICE_HPP
#define KIWI_UTILS_SLICE_HPP

#include <stdint.h>
#include <assert.h>

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

    Self operator()(int32_t s, int32_t e){
        return Self(_start+s, _end+e);
    }

    T* pointer() {
        return _start;
    }

    bool operator==(Range rhs) {
        return _start == rhs._start && _end == rhs._end;
    }

    bool contains(Range s) {
        return _start <= s._start && _end >= s._end;
    }

    bool isValid() {
        return _start <= _end && _start != nullptr;
    }

    void shrinkLeft(uint32_t num) {
        _start += num;
        assert (_start <= _end);
    }

    void shrinkRight(uint32_t num) {
        _end -= num;
        assert (_start <= _end);
    }

    Range<uint8_t> bytes() {
        return Range<uint8_t>(reinterpret_cast<uint8_t*>(_start), (_end - _start) * sizeof(_start) );
    }

    Self next() {
        Self s = *this;
        s.shrinkLeft();
        return s;
    }

    bool isValid() const {
        return !empty();
    }

    bool hasNext() const {
        return !next().empty();
    }

    // stl style interface

    int32_t size() const {
        return _end - _start;
    }

    int32_t empty() const {
        return size() == 0;
    }

    iterator begin() {
        return _start;
    }

    iterator end() {
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

private:
    T* _start;
    T* _end;
};

template<typename T> Range<T> slice(T* ptr, uint32_t size) {
    return Range<T>(ptr,size);
}

typedef Range<uint8_t> ByteRange;

#endif
