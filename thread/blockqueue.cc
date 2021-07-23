#include "blockqueue.h"

#include "mutext.h"

namespace recipes {
template <typename T>
bool BlockQueue<T>::add(T e) {
  MutexGuard lock_(mutex_);
  q_.emplace_back(e);
  cv_.notifyOne();
}

template <typename T>
bool BlockQueue<T>::add(T& e) {
  MutexGuard lock_(mutex_);
  q_.push_back(e);
  cv_.notifyOne();
}

template <typename T>
T BlockQueue<T>::take() {
  MutexGuard lock_(mutex_);
  while (q_.empty()) {
    cv_.wait();
  }
  T e = q_.front();
  q_.pop_front();
  return e;
}

}  // namespace recipes
