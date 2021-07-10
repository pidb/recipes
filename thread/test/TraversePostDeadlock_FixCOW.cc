#include <cassert>
#include <chrono>
#include <iostream>
#include <list>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

using std::lock_guard;
using std::mutex;
using std::thread;
using std::vector;

class Foo {
 public:
  Foo() = default;
  explicit Foo(int32_t id) : id_(id){};
  ~Foo() = default;
  void doit() const;

 private:
  int32_t id_;
};

typedef std::vector<Foo> FooList;
typedef std::shared_ptr<FooList> FooListPtr;
typedef vector<thread> ThreadList;

mutex g_mutex_;
ThreadList g_thread_list_;
FooListPtr g_foos_(new std::vector<Foo>);

void traverse() {
  FooListPtr newFoos;
  {
    lock_guard<mutex> lock_(g_mutex_);
    newFoos = g_foos_;
  }

  {
    lock_guard<mutex> lock_(g_mutex_);
    std::cout << "call traverse()" << std::endl;
  }
  for (auto it = newFoos->begin(); it != newFoos->end(); it++) {
    it->doit();
  }
}

void post(Foo f) {
  lock_guard<mutex> lock_(g_mutex_);

  if (!g_foos_.unique()) {
    std::cout << "call post() to copy on write" << std::endl;
    g_foos_.reset(new FooList(*g_foos_));
  }

  assert(g_foos_.unique());
  g_foos_->push_back(f);
}

void Foo::doit() const {
  std::lock_guard<mutex> lock_(g_mutex_);
  std::cout << "[" << id_ << "]"
            << " call doit()" << std::endl;
}

int main() {
  size_t threads_ = 5;
  for (size_t i = 0; i < threads_; i++) {
    g_thread_list_.push_back(thread(post, Foo(i)));
    g_thread_list_.push_back(thread(traverse));
  }

  for (thread& th_ : g_thread_list_) {
    th_.join();
  }
  assert(g_foos_->size() == threads_);
}