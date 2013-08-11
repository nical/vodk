// Copyright (c) 2013 Nicolas Silva
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef VODK_TIMELINE_HPP
#define VODK_TIMELINE_HPP

#include <stdint.h>
#include <assert.h>
#include <vector>
#include "vodk/core/dump.hpp"

namespace vodk {

template<typename T> class Timeline;
typedef float Time;

template<typename T>
struct TimelineNode
{
    TimelineNode(const T& d, Time t)
    : data(d)
    , time(t)
    {}

    T data;
    Time time;
};

template<typename T>
class Timeline
{
public:
    typedef Timeline<T> Self;
    typedef TimelineNode<T> Node;
    typedef uint32_t Index;
    typedef Node* Iterator;
    enum Flag { BEFORE, AFTER };

    Timeline() {}
    Timeline(Index capacity) {
        reserve(capacity);
    }

    void reserve(Index size) {
        _data.reserve(size);
    }

    Index get_index(Time t, Flag flag = BEFORE) const {
        if (_data.size() == 0) { return 0; }
        if (_data[0].time > t) { return 0; }
        for (unsigned i = 0; i < _data.size(); ++i) {
            if (_data[i].time <= t && i +1 < _data.size() && _data[i+1].time > t) {
                return flag==BEFORE ? i : i+1;
            }
        }
        return _data.size()-1;
    }

    const Node& get_node(Time t, Flag flag = BEFORE) const {
        return _data[get_index(t, flag)];
    }

    Node& get_node(Time t, Flag flag = BEFORE) {
        return _data[get_index(t, flag)];
    }

    const Node& get_nth_node(uint32_t nth) const {
        return _data[nth];
    }

    Node& get_nth_node(uint32_t nth) {
        return _data[nth];
    }

    T& get(Time t, Flag flag = BEFORE) const {
        return get_node(t, flag).data;
    }

    const T& get_nth(uint32_t nth) const {
        return get_nth_node(nth).data;
    }

    T& get_nth(uint32_t nth) {
        return get_nth_node(nth).data;
    }

    Time start_time() const {
        return size() > 0 ? _data[0].time : 0.0;
    }

    Time duration() const {
        return size() > 0 ? _data[size()-1].time - _data[0].time : 0.0;
    }

    void add(const T& toAdd, Time time) {
        if (_data.size() == 0) {
            _data.push_back(Node(toAdd, time));
            return;
        }
        Index idx = get_index(time, AFTER);
        if (_data[idx].time < time) {
            _data.push_back(Node(toAdd, time));
            return;
        }
        auto it = _data.begin();
        for (unsigned i = 0; i < idx; ++i) { ++it; }
        _data.insert(it, Node(toAdd, time));
    }

    void remove(Index idx) {
        auto it = _data.begin();
        for (unsigned i = 0; i < idx; ++i) { ++it; }
        _data.erase(it);
    }

    uint32_t size() const { return _data.size(); }

    void shift_time(Time offset) {
        auto it = _data.begin();
        auto stop = _data.end();
        for (; it != stop; ++it) {
            it->time += offset;
        }
    }

    void multiply_time(float factor) {
        auto it = _data.begin();
        auto stop = _data.end();
        for (; it != stop; ++it) {
            it->time * factor;
        }
    }

    void clear() {
        _data.clear();
    }

    Iterator begin() { return size() > 0 ? &_data[0] : nullptr; }

    Iterator end() { return size() > 0 ? &_data[0] + size() : nullptr; }

protected:
    std::vector<TimelineNode<T> > _data;
};

template<typename T>
struct LinearInterpolation
{
    static T compute(const T& a, const T& b, float t) {
        return a * (1.0 -t) + b * t;
    }
};

template<typename T, typename InterpolationFunc = LinearInterpolation<T> >
class InterpolatedTimeline : public Timeline<T>
{
public:
    typedef typename Timeline<T>::Node Node;
    typedef Timeline<T> Parent;
    T sample(Time t) {
        const Node& a = Parent::get_node(t, Parent::BEFORE);
        const Node& b = Parent::get_node(t, Parent::AFTER);
        if (t <= Parent::start_time()) return Parent::get_nth(0);
        if (t >= Parent::start_time() + Parent::duration()) return Parent::get_nth(0);
        if (&a == &b) return a.data;
        return InterpolationFunc::compute(a.data, b.data, (t - a.time)/(b.time - a.time));
    }
};

namespace debug {

template<typename T>
struct Dump<TimelineNode<T>> {
    static void dump(const TimelineNode<T>& n) {
        printf("[%f:", n.time);
        debug::dump(n.data);
        printf("]");
    }
};

template<typename T, typename I>
struct Dump<InterpolatedTimeline<T, I>> {
    static void dump(const Timeline<T>& tl) {
        printf("{<InterpolatedTimeline> ");
        for (unsigned i = 0; i < tl.size(); ++i) {
            debug::dump(tl.get_nth_node(i));
        }
        printf("}");
    }
};

template<typename T>
struct Dump<Timeline<T>> {
    static void dump(const Timeline<T>& tl) {
        printf("{<Timeline> ");
        for (unsigned i = 0; i < tl.size(); ++i) {
            debug::dump(tl.get_nth_node(i));
        }
        printf("}");
    }
};

} // debug
} // vodk

#endif
