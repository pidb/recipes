#ifndef RECIPES_BLOCK_QUEUE_H
#define RECIPES_BLOCK_QUEUE_H

#include <deque>

#include "condition.h"

namespace recipes {

template <typename T>
class BlockQueue {
 public:
  BlockQueue() : mutex_(), cv_(mutex_) {}
  bool add(T e);
  bool add(T& e);
  T take();

 private:
  std::deque<T> q_;
  MutexLock mutex_;
  ConditionVariable cv_;
};
}  // namespace recipes

#endif /* RECIPES_BLOCK_QUEUE_H */