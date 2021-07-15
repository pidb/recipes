#ifndef RECIPES_THREAD
#define RECIPES_THREAD

#include <unistd.h>

#include <cassert>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include "pthread.h"

using std::function;
using std::shared_ptr;
using std::string;
using std::weak_ptr;

namespace recipes {

typedef function<void()> thread_fn;

class thread {
 public:
  explicit thread(thread_fn fn, string name = string())
      : fn_(fn), name_(name), threadId_(0), tidPtr_(new pid_t(0)) {}
  void start();
  void join();
  pid_t get_tid();
  string get_name();

 private:
  thread_fn fn_;
  string name_;
  shared_ptr<pid_t> tidPtr_;
  pthread_t threadId_;
  bool started_;
  bool joined_;
  ;
};

class thread_proxy {
 public:
  explicit thread_proxy(string& name, thread_fn& fn, shared_ptr<pid_t>& tidPtr)
      : name_(name), fn_(fn), wpTid_(tidPtr) {}
  void runInthread();

 private:
  string name_;
  thread_fn fn_;
  weak_ptr<pid_t> wpTid_;
};

namespace this_thread {
pid_t get_id();
string get_name();
}  // namespace this_thread

}  // namespace recipes

#endif /* RECIPES_THREAD */