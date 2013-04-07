#ifndef KIWI_UTILS_SLICE_HPP
#define KIWI_UTILS_SLICE_HPP

#include <stdint.h>
#include <assert.h>

template<typename T>
class Slice {
public:
    typedef T ValueType;
    typedef Slice<T> Self;
    typedef T* iterator;
    typedef const T* const_iterator;

    Slice(T* s, T* e) : _start(s), _end(e) {}
    Slice(T* aStart, uint32_t aSize) : _start(aStart), _end(aStart+aSize) {}

    T& operator[](uint32_t i) {
        assert(i < size());
        return _start[i];
    }

    Self operator()(int32_t s, int32_t e){
        return Self(_start+s, _end+e);
    }

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

    T* pointer() {
        return _start;
    }

    bool operator==(Slice rhs) {
        return _start == rhs._start && _end == rhs._end;
    }

    bool contains(Slice s) {
        return _start <= s._start && _end >= s._end;
    }

    bool isValid() {
        return _start <= _end && _start != nullptr;
    }

    T& pop_front() {
        assert(_start < _end);
        return *_start++;
    }

    T& pop_back() {
        assert(_start < _end);
        return *--_end;
    }

    void shrinkLeft(uint32_t num) {
        _start += num;
        assert (_start <= _end);
    }

    void shrinkRight(uint32_t num) {
        _end -= num;
        assert (_start <= _end);
    }

    Slice<uint8_t> bytes() {
        return Slice<uint8_t>(reinterpret_cast<uint8_t*>(_start), (_end - _start) * sizeof(_start) );
    }
private:
    T* _start;
    T* _end;
};

template<typename T> Slice<T> slice(T* ptr, uint32_t size) {
    return Slice<T>(ptr,size);
}

typedef Slice<uint8_t> ByteSlice;

#endif
