#ifndef RECIPES_CONDITION_VARIABLE_H
#define RECIPES_CONDITION_VARIABLE_H

#include <errno.h>
#include <pthread.h>

#include "base.h"
#include "mutext.h"

namespace recipes {
class ConditionVariable : base::nocopyable {
 public:
  explicit ConditionVariable(MutexLock& mutexLock) : mutex_(mutexLock) {
    pthread_cond_init(&cond_, nullptr);
  }
  ~ConditionVariable() {
    std::cout << "~ConditionVariable" << std::endl;
    pthread_cond_destroy(&cond_);
  }
  void wait() { pthread_cond_wait(&cond_, &mutex_); }
  bool waitForSeconds(time_t waitSeconds) {
    // TODO may be validator waitSeconds.
    timespec abstime;
    clock_gettime(CLOCK_REALTIME, &abstime);
    abstime.tv_sec += waitSeconds;
    return ETIMEDOUT == pthread_cond_timedwait(&cond_, &mutex_, &abstime);
  }
  void notifyOne() { pthread_cond_signal(&cond_); }
  void notifyAll() { pthread_cond_broadcast(&cond_); }

 private:
  pthread_cond_t cond_;
  MutexLock& mutex_;
};
}  // namespace recipes

#endif /* RECIPES_CONDITION_VARIABLE_H */