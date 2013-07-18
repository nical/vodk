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

    Index getIndex(Time t, Flag flag = BEFORE) const {
        if (_data.size() == 0) { return 0; }
        if (_data[0].time > t) { return 0; }
        for (unsigned i = 0; i < _data.size(); ++i) {
            if (_data[i].time <= t && i +1 < _data.size() && _data[i+1].time > t) {
                return flag==BEFORE ? i : i+1;
            }
        }
        return _data.size()-1;
    }

    const Node& getNode(Time t, Flag flag = BEFORE) const {
        return _data[getIndex(t, flag)];
    }

    Node& getNode(Time t, Flag flag = BEFORE) {
        return _data[getIndex(t, flag)];
    }

    const Node& getNthNode(uint32_t nth) const {
        return _data[nth];
    }

    Node& getNthNode(uint32_t nth) {
        return _data[nth];
    }

    T& get(Time t, Flag flag = BEFORE) const {
        return getNode(t, flag).data;
    }

    const T& getNth(uint32_t nth) const {
        return getNthNode(nth).data;
    }

    T& getNth(uint32_t nth) {
        return getNthNode(nth).data;
    }

    Time startTime() const {
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
        Index idx = getIndex(time, AFTER);
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

    void shiftTime(Time offset) {
        auto it = _data.begin();
        auto stop = _data.end();
        for (; it != stop; ++it) {
            it->time += offset;
        }
    }

    void multiplyTime(float factor) {
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
        const Node& a = Parent::getNode(t, Parent::BEFORE);
        const Node& b = Parent::getNode(t, Parent::AFTER);
        if (t <= Parent::startTime()) return Parent::getNth(0);
        if (t >= Parent::startTime() + Parent::duration()) return Parent::getNth(0);
        if (&a == &b) return a.data;
        return InterpolationFunc::compute(a.data, b.data, (t - a.time)/(b.time - a.time));
    }
};

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
}

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
            debug::dump(tl.getNthNode(i));
        }
        printf("}");
    }
};

template<typename T>
struct Dump<Timeline<T>> {
    static void dump(const Timeline<T>& tl) {
        printf("{<Timeline> ");
        for (unsigned i = 0; i < tl.size(); ++i) {
            debug::dump(tl.getNthNode(i));
        }
        printf("}");
    }
};

} // debug
} // vodk

#endif
