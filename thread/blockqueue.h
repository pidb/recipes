#ifndef RECIPES_BLOCK_QUEUE_H
#define RECIPES_BLOCK_QUEUE_H

#include <deque>

#include "base.h"
#include "condition.h"

namespace recipes {

template <typename T>
class BlockQueue;

template <typename T>
class BlockQueue : base::nocopyable {
 public:
  BlockQueue() : mutex_(), cv_(mutex_) {}
  bool add(T e) {
    MutexGuard lock_(mutex_);
    q_.push_back(e);
    cv_.notifyOne();
    return true;
  }

  bool add(T& e) {
    MutexGuard lock_(mutex_);
    q_.push_back(e);
    cv_.notifyOne();
    return true;
  }

  T take() {
    MutexGuard lock_(mutex_);
    while (q_.empty()) {
      cv_.wait();
    }
    T e = q_.front();
    q_.pop_front();
    return e;
  }

 private:
  std::deque<T> q_;
  MutexLock mutex_;
  ConditionVariable cv_;
};

}  // namespace recipes

#endif /* RECIPES_BLOCK_QUEUE_H */