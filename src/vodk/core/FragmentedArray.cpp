
#ifdef VODK_TEST_MAIN

#include "vodk/core/FragmentedArray.hpp"
#include <stdio.h>

namespace vodk {

bool test_FragmentedArray() {
    printf("Begin test - vodk::FragmentedArray\n");
    FragmentedArray<int,8> a;

    assert(a.getSize() == 0);
    assert(a.getCapacity() == 0);


    for (int i = 0; i < 100; ++i) {
        a.pushBack(i);
        assert(a.getSize() == i+1);
    }
    for (int i = 0; i < 100; ++i) {
        assert(a.at(i) == i);
    }
    assert(a.getSize() == 100);
    assert(a.getCapacity() >= 100);

    a.clear();

    assert(a.getSize() == 0);

    for (int i = 0; i < 100; ++i) {
        a.pushBack(i);
        assert(a.getSize() == i+1);
    }
    for (int i = 0; i < 100; ++i) {
        assert(a.at(i) == i);
    }
    assert(a.getSize() == 100);
    assert(a.getCapacity() >= 100);

    printf("End test - vodk::FragmentedArray\n");
    return true;
}

} // vodk

int main() {
    return vodk::test_FragmentedArray() ? 0 : 1;
}

#endif