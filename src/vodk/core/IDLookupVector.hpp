#ifndef VODK_CORE_IDLOOKUPTABLE_HPP
#define VODK_CORE_IDLOOKUPTABLE_HPP

#include <vector>
#include <assert.h>
#include <stdint.h>
#include <limits>
#include <stdio.h>

#include "vodk/core/dump.hpp"
#include "vodk/core/Range.hpp"

namespace vodk {

template<typename T, typename Index = uint16_t>
class IDLookupVector
{
public:
    typedef Index Offset;
    struct IndexIterator {
        IndexIterator(const Index* pidx) : _idx(const_cast<Index*>(pidx)) {}
        Index* _idx;
        IndexIterator operator ++() {
            while( *(++_idx) == INVALID_INDEX) {}
            return *this;
        }
        Index& operator *() { return *_idx; }
        Index& operator->() { return *_idx; }
        bool operator ==(const IndexIterator& rhs) const { return _idx == rhs._idx; }
        bool operator !=(const IndexIterator& rhs) const { return _idx != rhs._idx; }
    };
    static const Index INVALID_INDEX = std::numeric_limits<Index>::max();

    uint32_t size() const { return _data.size(); }

	IDLookupVector(Index preallocate = 64)
	{
		_data.reserve(preallocate);
		_indices.reserve(preallocate);
	}

    bool contains(Index idx) const {
        if (idx > _indices.size()) return false;
        return _indices[idx] != INVALID_INDEX;
    }

    const T& get(Index index) const {
        assert(contains(index));
        return _data[_indices[index]];
    }

	T& get(Index index) {
		assert(contains(index));
		return _data[_indices[index]];
	}

    const T& get(IndexIterator it) const {
        assert(*it != INVALID_INDEX);
        return _data[*it];
    }

    T& get(IndexIterator it) {
        assert(*it != INVALID_INDEX);
        return _data[*it];
    }

	Index add(const T& obj) {
		_data.push_back(obj);
		Offset offset = _data.size() - 1;
        Index idx = 0;
        while (idx < _indices.size() && _indices[idx] != INVALID_INDEX) {
            ++idx;
        }
        if (idx == _indices.size()) {
            _indices.push_back(offset);
        } else {
            _indices[idx] = offset;
        }
        _data[_data.size() - 1].index = idx;
		return idx;
	}

    void remove(Index index) {
        if (!contains(index)) return;
        Offset offset = _indices[index];
        // swap with the last element
        _data[offset] = _data[_data.size() -1];
        // find the id of the element we swaped and fix it's index
        _indices[_data[offset].index] = offset;
        // reduce the vector size
        _data.resize(_data.size()-1);
        // mark the index that has been removed as invalid
        _indices[index] = INVALID_INDEX;
    }

    Range<T> range() {
        return Range<T>(_data.data(), _data.size());
    }

    IndexIterator indices_begin() const {
        return IndexIterator(_indices.size() > 0 ? &_indices[0] : nullptr);
    }

    IndexIterator indices_end() const {
        return IndexIterator(_indices.size() > 0 ? &_indices[_indices.size()-1]+1 : nullptr);
    }

protected:
	std::vector<Index> _indices;
	std::vector<T> _data;
};



namespace debug {

template<typename T, typename ID>
struct Dump<vodk::IDLookupVector<T,ID>> {
    static void dump(const IDLookupVector<T,ID>& val) {
        printf("{<IDLookupVector> ");
        auto start = val.indices_begin();
        auto stop = val.indices_end();
        for (auto it = start; it != stop; ++it) {
            printf("[%i:", &(*it) - &(*start));
            debug::dump<T>(val.get(it));
        }
    }
};

} // debug

namespace unittest {
void IDLookupVector();
} // unittest

} // vodk

#endif
