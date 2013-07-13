#ifndef VODK_CORE_IDLOOKUPTABLE_HPP
#define VODK_CORE_IDLOOKUPTABLE_HPP

#include <vector>
#include <assert.h>
#include <stdint.h>

namespace vodk {
// TODO sort out the index mask thing
template<typename T, typename ID>
class IDLookupVector
{
public:
	typedef ID ObjectID;
	typedef typename ObjectID::Index Index;
	static const Index INDEX_MASK = 1 << 15;

	IDLookupVector(Index preallocate)
	{
		_data.reserve(preallocate);
		_indices.reserve(preallocate);
		_freeList = 0 | INDEX_MASK;
	}

	bool contains(ObjectID id) const {
		if (id.index > _indices.length()) return false;
		// 
		return !(_indices[id.index] & INDEX_MASK);
	}

	T& get(ObjectID id) {
		assert(id.index < _indices.size());
		_data[_indices[id.index]];
	}

	Index add(const T& obj) {
		_data.push_back(obj);
		Index objIndex = _data.size() - 1;
		Index result = _freeList;
		// read the free list
		_freeList = _indices[_freeList &~INDEX_MASK];
		// it is now an actual index
		_indices[result] = objIndex;
		return result;
	}

	void remove(ObjectID id) {
		if (!contains(id)) return;
		Index index = _indices[id];
		// swap with the last element
		_data[index] = _data[_data.size() -1];
		// reduce the vector size
		_data.resize(_data.size()-1);
		// find the id of the element we swaped and fix it's index!
		_indices[_data[index].index] = id.index;
		// update the freelist
		_indices[id.index] = _freeList | INDEX_MASK;
		_freeList = id.index | INDEX_MASK;
	}
protected:
	std::vector<Index> _indices;
	std::vector<T> _data;
	Index _freeList;
};



/*
template<typename T> struct ArrayTrait {};template<typename T, int N>
struct ArrayTrait<FragmentedArray<T,N> >
{
	typedef typename T ValueType;
};
template<typename ArrayType>
class FreeListArray
{
public:
	typedef ArrayType Array;
	typedef typename ArrayTrait<ArrayType>::ValueType ValueType;
	typedef uint32_t Index;
	typedef Index FreeList;

	Index add(const ValueType& toAdd) {
		if (_freeList) {
			Index idx = _freeList;
			_freeList = asFreeList(_array[_freeList]);
			_array[idx] = toAdd;
			return idx;
		}
		_array.push_back(toAdd);
		return _array.size() - 1;
	}

	void remove(Index idx) {
		FreeList it = _freeList;
	}

private:
	FreeList& asFreeList(ValueType& v) {
		return *reinterpret_cast<FreeList*>(&v);
	}
	FreeList& freeListNext(FreeList idx) {
		return asFreeList(_array[idx]);
	}
	Array _array;
	FreeList _freeList;
};


template<typename T, int N>
class IDLookupTable
{
public:

protected:
};

template<typename T, int N>
struct ArrayTrait<FragmentedArray<T,N> >
{
	typedef typename T ValueType;
};

template<typename T, int N>
struct ArrayTrait<std::vector<T> >
{
	typedef typename T ValueType;
};
*/


} // vodk

#endif
