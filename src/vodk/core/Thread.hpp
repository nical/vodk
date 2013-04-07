
#ifndef VODK_CORE_TREAD
#define VODK_CORE_TREAD

/*
#include <glib.h>
#include <stdint.h>

namespace core {

class Thread;

struct ThreadData {
    virtual ~ThreadData() {}
};

struct Task {
    virtual ~Task() {}
    virtual bool run(Thread* athread) = 0;
};

class MessageLoop
{
public:
    MessageLoop();
    ~MessageLoop();

    void enqueue(Task* aMsg);
    Task* dequeue();

    GAsyncQueue* _queue;
};

class Thread {
public:
    Thread(const char* aName, ThreadData* aData = nullptr)
    : _name(aName), _data(aData) {}

    Thread(GThread* aThread, 
          const char* aName,
          ThreadData* aData = nullptr)
    : _thread(aThread), _name(aName), _data(aData) {
        g_thread_ref(_thread);
    }

    void start();
    void run();
    void send(Task* aTask);
    void sendStop();
    void join();

    const char* getName() const {
        return _name;
    }
    ThreadData* getData() const {
        return _data;
    }

    static GThread* thisThread();
protected:
    GThread* _thread;
    MessageLoop _msgLoop;
    ThreadData* _data;
    const char* _name;
};


} // namespace
*/
#endif
