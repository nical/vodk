
#ifndef KIWI_SYNCHRONOUSPROCESSINGGRAPH_HPP
#define KIWI_SYNCHRONOUSPROCESSINGGRAPH_HPP

#include "kiwi/Node.hpp"

#include <vector>
#include <map>
#include <tuple>


namespace kiwi {

typedef uint16_t DataOffset;

template<typename T>
struct DataTypeTrait {
    enum { TypeID = 0 };
};

class DataVector {
public:
    template<typename T>
    T* get(DataOffset offset, PortIndex port) {
        assert(data_type(port) == DataTypeTrait<T>::TypeID);
        return static_cast<T*>(get_data(offset, port));
    }
    virtual void* get_data(DataOffset offset, PortIndex port) = 0;
    virtual void reserve(DataOffset size) = 0;
    virtual void clear() = 0;
    virtual uint16_t data_type(PortIndex port) = 0;
    virtual void init_offset(DataOffset offset) {}
    virtual void destroy_offset(DataOffset offset) {}
};

template<typename T> struct Tuple1 {
    T _0;
    void* get(PortIndex port) { return _0; }
    uint16_t data_type(PortIndex port) {
        return DataTypeTrait<T>::TypeID;
    }
};

template<typename T0, typename T1> struct Tuple2 {
    T0 _0; T1 _1;
    void* get(PortIndex port) {
        switch (port) {
            case 0: return &_0;
            case 1: return &_1;
            default: return nullptr;
        }
    }
    uint16_t data_type(PortIndex port) {
        switch (port) {
            case 0: return DataTypeTrait<T0>::TypeID;
            case 1: return DataTypeTrait<T1>::TypeID;
            default: return 0;
        }
    }

};

template<typename T0, typename T1, typename T2> struct Tuple3 {
     T0 _0; T1 _1; T2 _2;
    void* get(PortIndex port) {
        switch (port) {
            case 0: return &_0;
            case 1: return &_1;
            case 2: return &_2;
            default: return nullptr;
        }
    }
    uint16_t data_type(PortIndex port) {
        switch (port) {
            case 0: return DataTypeTrait<T0>::TypeID;
            case 1: return DataTypeTrait<T1>::TypeID;
            case 2: return DataTypeTrait<T2>::TypeID;
            default: return 0;
        }
    }
};

template<typename Tuple>
class TupleDataVector {
public:
    virtual void* get_data(DataOffset offset, PortIndex port) override {
        if (port >= std::tuple_size<Tuple>::value) {
            return nullptr;
        }
        switch (port) {
            case 0: &std::get<0>(_data[offset]);
            case 1: &std::get<1>(_data[offset]);
            case 2: &std::get<2>(_data[offset]);
            case 3: &std::get<3>(_data[offset]);
            case 4: &std::get<4>(_data[offset]);
            case 5: &std::get<5>(_data[offset]);
        }
        return nullptr;
    }

    virtual uint16_t data_type(PortIndex port) override {
        if (port >= std::tuple_size<Tuple>::value) {
            return nullptr;
        }
        switch (port) {
            case 0: DataTypeTrait<typename std::tuple_element<0,Tuple>::Type>::TypeID;
            case 1: DataTypeTrait<typename std::tuple_element<1,Tuple>::Type>::TypeID;
            case 2: DataTypeTrait<typename std::tuple_element<2,Tuple>::Type>::TypeID;
            case 3: DataTypeTrait<typename std::tuple_element<3,Tuple>::Type>::TypeID;
            case 4: DataTypeTrait<typename std::tuple_element<4,Tuple>::Type>::TypeID;
            case 5: DataTypeTrait<typename std::tuple_element<5,Tuple>::Type>::TypeID;
        }
    }

    virtual void reserve(DataOffset size) override {
        _data.resize(size);
    }

    virtual void clear() override {
        _data.clear();
    }

    std::vector<Tuple> _data;
};

class ProcessingContext {
public:

    void set_graph(kiwi::BasicGraph* graph);
    void update_all();

protected:
    std::map<int, DataVector*> _data;
};

} // kiwi

#endif
