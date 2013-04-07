
#include "Thread.hpp"
/*
namespace core {

MessageLoop::MessageLoop() {
    _queue = g_async_queue_new();
}

MessageLoop::~MessageLoop() {
    g_async_queue_unref(_queue);
}

void MessageLoop::enqueue(Task* msg) {
    g_async_queue_push(_queue, msg);    
}

Task* MessageLoop::dequeue() {
    return static_cast<core::Task*>(g_async_queue_pop(_queue));
}

static gpointer ThreadRun(gpointer data) {
    static_cast<core::Thread*>(data)->run();
    return nullptr;
}

void Thread::start() {
    _thread = g_thread_new(_name, &ThreadRun, this);
}

void Thread::send(Task* msg) {
    _msgLoop.enqueue(msg);
}

void Thread::run() {
    bool stop = false;
    while (!stop) {
        Task* task = _msgLoop.dequeue();
        if (task) {
            if (!task->run(this)) {
                stop = true;
            }
        }
        delete task;
    }
}

struct StopTask : public core::Task {
    bool run(Thread*) { return false; }
};

void Thread::sendStop() {
    send(new StopTask());
}
void Thread::join() {
    g_thread_join(_thread);

}

GThread* thisThread() {
    return g_thread_self();
}

} // namespace
*/