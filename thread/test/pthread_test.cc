#include <unistd.h>
#include <iostream>
#include <pthread.h>
#include <string>
#include <functional>


__thread pid_t t_cached_id;

pid_t get_id() {
    if (t_cached_id) {
        return t_cached_id;
    }

    t_cached_id = gettid();
    return t_cached_id;
}


typedef std::function<void()> thread_fn;

struct ThreadData {
    pid_t *pid_;
    std::string name_;
    thread_fn& fn_;
public:
    ThreadData(thread_fn& fn, std::string name) :fn_(fn), name_(name) {}
    void runInThread() {
        pid_t tid = get_id();
        *pid_ = tid;
        fn_();
    }
};

void* startThread(void *data) 
{
    ThreadData* td = static_cast<ThreadData*> (data);
    td->runInThread();
    delete data;
    return nullptr;
}
 

class thread {
public:
    explicit thread(thread_fn& fn, std::string name = std::string())
    :fn_(fn), name_(std::move(name)),pt(0) {};

    void start() {
        ThreadData *data = new ThreadData(fn_, name_);
        if (pthread_create(&pt, nullptr, &startThread, data)) {
            delete data;
            abort();
        }
    }
private:
    pid_t *pid_;
    std::string name_;
    thread_fn& fn_;
    pthread_t pt;
};

void threadFn(int i) {
   std::cout << i << std::endl;

}

int main() 
{
    int i = 1;
    thread t1(std::bind(threadFn, std::ref(i)), "hell i");

    std::cout << gettid() << std::endl;
}