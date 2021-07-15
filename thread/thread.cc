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
  fn_();  // TODO allow capture return value
  threadCachedName_ = "finished";
}

void* startThread(void* data) {
  thread_proxy* proxy = static_cast<thread_proxy*>(data);
  proxy->runInthread();  // TODO proxy return value
  delete proxy;
  return nullptr;
}

void thread::start() {
  assert(!started_ && !joined_);
  started_ = true;

  thread_proxy* proxy = new thread_proxy(name_, fn_, tidPtr_);
  int err = pthread_create(&threadId_, nullptr, &startThread, proxy);
  if (err != 0) {
    started_ = false;
    delete proxy;
    handle_error(err, "pthread_create");
    abort();
  }
}

void thread::join() {
  assert(started_ && !joined_);
  // TODO recevie return value from pthread.
  joined_ = true;
  int err = -1;
  auto observeReturnValue = wkReturnValuePtr_.lock();
  if (observeReturnValue) {
    err = pthread_join(threadId_, &(*observeReturnValue));
  } else {
    err = pthread_join(threadId_, nullptr);
  }
  if (err != 0) {
    joined_ = false;
    handle_error(err, "pthread_join");
  }
}

void thread::handle_error(error_t en, string msg) {
  errno = en;
  std::perror(msg.c_str());
}

}  // namespace recipes