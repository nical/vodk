
#include "vodk/core/Timeline.hpp"
#include "vodk/core/FreelistVector.hpp"
#include "vodk/core/IDLookupVector.hpp"
#include <string>

namespace vodk {

namespace debug {
struct IDObj {
    IDObj(int val = 0) : value(val), index(0) {}
    int value;
    uint16_t index;
};

template<>
struct Dump<vodk::debug::IDObj> {
    static void dump(const vodk::debug::IDObj& val) {
        debug::dump<int>(val.value);
    }
};
} // debug


namespace unittest {

void Timeline() {
    printf("vodk::unittest::Timeline\n");
    InterpolatedTimeline<float> timeline;
    timeline.add(1.0, 1.0);
    timeline.add(2.0, 2.0);
    timeline.add(4.0, 4.0);
    timeline.add(3.0, 3.0);
    timeline.add(7.0, 7.0);
    timeline.add(0.0, 0.0);
    timeline.add(6.0, 6.0);
    debug::dumpln(timeline);
    for (float f = 0.0; f < 6.1; f += 0.5) {
        printf("sample %f -> ", f);
        debug::dumpln(timeline.sample(f));
        assert(timeline.sample(f) == f);
    }
}

struct FVFoo {
	uint32_t value;
};

void FreelistVector() {
	vodk::FreelistVector<std::string> v1;
	vodk::FreelistVector<FVFoo> v2;

	auto id0 = v1.add("0");
	auto id1 = v1.add("1");
	auto id2 = v1.add("2");
	auto id3 = v1.add("3");
	auto id4 = v1.add("4");
	auto id5 = v1.add("5");

	assert(v1.size() == 6);
	assert(v1[id0] == std::string("0"));
	assert(v1[id1] == std::string("1"));
	assert(v1[id2] == std::string("2"));
	assert(v1[id3] == std::string("3"));
	assert(v1[id4] == std::string("4"));
	assert(v1[id5] == std::string("5"));

    v1.remove(id1);
    v1.remove(id0);

    auto id6 = v1.add("6");
    auto id7 = v1.add("7");
    auto id8 = v1.add("8");

    assert(v1[id2] == std::string("2"));
    assert(v1[id3] == std::string("3"));
    assert(v1[id4] == std::string("4"));
    assert(v1[id5] == std::string("5"));
    assert(v1[id6] == std::string("6"));
    assert(v1[id7] == std::string("7"));
    assert(v1[id8] == std::string("8"));
}

using namespace vodk::debug;
void IDLookupVector() {
    vodk::IDLookupVector<IDObj> vec;

    uint16_t idx1 = vec.add(IDObj(1));
    uint16_t idx2 = vec.add(IDObj(2));
    vec.remove(idx1);
    uint16_t idx3 = vec.add(IDObj(3));
    uint16_t idx4 = vec.add(IDObj(4));
    vec.remove(idx3);
    uint16_t idx5 = vec.add(IDObj(5));
    vec.remove(idx2);
    dumpln(vec.get(idx4).value);
    assert(vec.get(idx4).value == 4);
    dumpln(vec.get(idx5).value);
    assert(vec.get(idx5).value == 5);
    debug::dump(vec);
}


} // unittest
} // vodk
