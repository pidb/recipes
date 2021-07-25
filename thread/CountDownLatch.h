#ifndef RECIPES_COUNT_DOWN_LATCH_H
#define RECIPES_COUNT_DOWN_LATCH_H

#include "condition.h"
#include "mutext.h"

namespace recipes {

class CountDownLatch : recipes::base::nocopyable {
 public:
  explicit CountDownLatch(size_t count)
      : mutex_(), cv_(mutex_), count_(count) {}

  void Wait() {
    MutexGuard lock_(mutex_);
    while (count_ > 0) {
      cv_.wait();
    }
  }

  void Done() {
    MutexGuard lock_(mutex_);
    --count_;
    if (count_ == 0) {
      cv_.notifyAll();
    }
  }

 private:
  mutable MutexLock mutex_;
  ConditionVariable cv_;
  size_t count_;
};

}  // namespace recipes

#endif /* RECIPES_COUNT_DOWN_LATCH_H */
