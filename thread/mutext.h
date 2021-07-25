
#ifndef RECIPES_MUTEX_H
#define RECIPES_MUTEX_H
#include <pthread.h>

#include <cstdlib>

#include "base.h"
#include "thread.h"

namespace recipes {

class MutexLock final : base::nocopyable {
 public:
  MutexLock() : owner_(0) {
    if (pthread_mutex_init(&mutex_variable_, nullptr) != 0) {
      abort();
    }
  }

  ~MutexLock() {
    std::cout << "~MutexLock" << std::endl;
    assert(owner_ == 0);
    pthread_mutex_destroy(&mutex_variable_);
  }

  void lock() {
    pthread_mutex_lock(&mutex_variable_);
    owner_ = recipes::this_thread::get_id();
  }

  void unlock() {
    owner_ = 0;
    pthread_mutex_unlock(&mutex_variable_);
  }

  bool is_owner_thread() { return owner_ == this_thread::get_id(); }

  pthread_mutex_t* operator&() { return &mutex_variable_; }

 private:
  pthread_mutex_t mutex_variable_;
  pid_t owner_;
};

class MutexGuard : base::nocopyable {
 public:
  explicit MutexGuard(MutexLock& mutex) : mutex_(mutex) { mutex_.lock(); }

  ~MutexGuard() { mutex_.unlock(); };

 private:
  MutexLock& mutex_;
};

};  // namespace recipes

// Prevent misuse like:
// MutexLockGuard(mutex_);
// A tempory object doesn't hold the lock for long!
#define MutexGuard(x) error "Missing guard object name"

#endif /* RECIPES_MUTEX_H */