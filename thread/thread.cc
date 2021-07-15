#include "./thread.h"

namespace recipes {
__thread const char* threadCachedName_ = "unknow";
__thread pid_t threadCachedTid_;

namespace this_thread {
pid_t get_id() {
  if (!threadCachedTid_) {
    threadCachedTid_ = gettid();
  }
  return threadCachedTid_;
}

string get_name() { return string(threadCachedName_); }
}  // namespace this_thread

void thread_proxy::runInthread() {
  auto tidPtr = wpTid_.lock();
  if (tidPtr) {
    *tidPtr = this_thread::get_id();
  }

  threadCachedName_ = name_.empty() ? "anonymous" : name_.c_str();
  fn_();
  threadCachedName_ = "finished";
}

void* startThread(void* data) {
  thread_proxy* proxy = static_cast<thread_proxy*>(data);
  proxy->runInthread();
  delete proxy;
}

void thread::start() {
  assert(!started_ && !joined_);
  started_ = true;

  thread_proxy* proxy = new thread_proxy(name_, fn_, tidPtr_);
  if (pthread_create(&threadId_, nullptr, &startThread, proxy) != 0) {
    started_ = false;
    delete proxy;
    abort();
  }
}

void thread::join() {
  assert(started_ && !joined_);
  // TODO recevie return value from pthread.
  if (pthread_join(threadId_, nullptr) != 0) {
    joined_ = true;
  }
}

}  // namespace recipes