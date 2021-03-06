#ifndef RECIPES_THREAD
#define RECIPES_THREAD

#include <unistd.h>

#include <cassert>
#include <cerrno>
#include <cstdio>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include "base.h"
#include "pthread.h"

using std::function;
using std::shared_ptr;
using std::string;
using std::weak_ptr;

namespace recipes {

typedef function<void()> thread_fn;

class thread : base::nocopyable {
 public:
  explicit thread(thread_fn fn, string name)
      : fn_(fn), name_(name), tidPtr_(new pid_t(0)), threadId_(0) {}

  ~thread();

  void start();
  void join();
  pid_t get_tid();
  string get_name();

 private:
  void handle_error(error_t en, string msg);

  thread_fn fn_;
  string name_;
  shared_ptr<pid_t> tidPtr_;
  pthread_t threadId_;
  bool started_;
  bool joined_;
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