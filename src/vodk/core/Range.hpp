#ifndef KIWI_UTILS_SLICE_HPP
#define KIWI_UTILS_SLICE_HPP

#include <stdint.h>
#include <assert.h>

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
    bool isValid() const {
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

    Range<uint8_t> bytes() const {
        return Range<uint8_t>(reinterpret_cast<uint8_t*>(_start), (_end - _start) * sizeof(_start) );
    }

    Self next() const {
        Self s = *this;
        s.shrinkLeft();
        return s;
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

template<typename T> Range<T> slice(T* ptr, uint32_t size) {
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

    bool isValid() {
        return _range.isValid();
    }

    void shrinkLeft(uint32_t num) {
        _range.shrinkLeft(num*_step);
    }

    void shrinkRight(uint32_t num) {
        _range.shrinkRight(num*_step);
    }

    Range<uint8_t> bytes() {
        return _range;
    }

    Self next() const {
        Self s = *this;
        s.shrinkLeft(1);
        return s;
    }

    bool isValid() const {
        return _range.isValid();
    }

    bool hasNext() const {
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
        shrinkLeft(1);
        return start;
    }

    void* pop_back() {
        void* end = _range._end;
        shrinkRight(1);
        return end;
    }
private:
    ByteRange _range;
    uint8_t _step; // in bytes
};

#endif
