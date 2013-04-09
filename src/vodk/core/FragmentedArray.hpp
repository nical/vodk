
#ifndef VODKA_CORE_FRAGMENTEDARRAY_HPP
#define VODKA_CORE_FRAGMENTEDARRAY_HPP

#include <stdint.h>
#include <assert.h>

namespace vodk {

template<typename T, int N>
class FragmentedArray {
private:
    struct Chunk {
        Chunk() : _next(nullptr), _size(0) {}
        T _data[N];
        Chunk* _next;
        uint32_t _size;
    };

public:
    uint32_t getSize() const {
        Chunk* it = _firstChunk;
        uint32_t size = 0;
        while (it && (it->_next != nullptr) && (it->_size == N)) {
            it = it->_next;
            size += N;
        }
        if (it) size += it->_size;
        return size;
    }

    uint32_t getCapacity() const {
        Chunk* it = _firstChunk;
        uint32_t capacity = 0;
        while (it) {
            it = it->_next;
            capacity += N;
        }
        return capacity;
    }

    void setCapacity(uint32_t newCapacity) const {
        // TODO
    }

    T& at(uint32_t index) {
        uint32_t nChunks = index / N;
        uint32_t rem = index % N;
        Chunk* it = _firstChunk;
        int i = 0;
        while(it && i < nChunks) {
            it = it->_next;
            ++i;
        }
        assert(it);
        assert(rem < it->_size);
        return it->_data[rem];
    }

    void pushBack(const T& value) {
        Chunk*& c = lastUsedChunk();
        if (!c) {
            c = new Chunk();
        }
        Chunk* vc = c;
        if (c->_size == N) {
            if (c->_next == nullptr) {
                c->_next = new Chunk;
            }
            vc = c->_next;
        }
        vc->_data[vc->_size] = value;
        ++vc->_size;
    }

    T popBack() {
        Chunk* c = lastUsedChunk();
        assert(c != nullptr);
        assert(c->_size != 0);
        T val = c->_data[c->_size - 1];
        --c->_size;
        return val;
    }

    void clear(bool deallocate = true) {
        if (_firstChunk) {
            recClearChuck(_firstChunk, true);
        }
        if (deallocate) {
            _firstChunk = nullptr;
        }
    }

private:
    void recClearChuck(Chunk* c, bool deallocate) {
        if (c->_next) {
            recClearChuck(c->_next, deallocate);
        }
        if (deallocate) {
            delete c;
        } else {
            c->_size = 0;
        }
    }
    Chunk*& lastUsedChunk() {
        Chunk** it = &_firstChunk;
        while(*it && (*it)->_size == N &&
              (*it)->_next && (*it)->_next->_size != 0) {
            it = &(*it)->_next;
        }
        return *it;
    }

    Chunk* _firstChunk;
};

} // vodk

#endif
