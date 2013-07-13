#ifndef VODK_CORE_DUMP_HPP
#define VODK_CORE_DUMP_HPP

#include <stdio.h>

namespace vodk {
namespace debug {


template<typename T> struct Dump {
    static void dump(const T& val) {printf("<unserialized>");}
};

template<> struct Dump<float> { static void dump(const float& val) {printf("%f", val);} };
template<> struct Dump<int> { static void dump(const int& val) {printf("%i", val);} };
template<> struct Dump<uint32_t> { static void dump(const uint32_t& val) {printf("%i", (int)val);} };

template<typename T> void dump(const T& val) {
    Dump<T>::dump(val);
}

template<typename T> void dumpln(const T& val) {
    dump(val);
    printf("\n");
}

} // debug
} // vodk

#endif
