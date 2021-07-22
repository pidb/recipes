#include <functional>
#include <iostream>
#include <memory>
#include <string>

#include "mutext.h"
#include "thread.h"

recipes::MutexLock g_mutex_;

void threadWithOutput(int i) {
  recipes::MutexGuard lock_(g_mutex_);
  std::cout << "id=" << recipes::this_thread::get_id()
            << ", name=" << recipes::this_thread::get_name() << " output=" << i
            << std::endl;
}

int main() {
  recipes::thread* t1 = new recipes::thread(
      [] {
        recipes::MutexGuard lock_(g_mutex_);

        std::cout << "is the lock owner: " << g_mutex_.is_owner_thread()
                  << std::endl;

        std::cout << "id=" << recipes::this_thread::get_id()
                  << ", name=" << recipes::this_thread::get_name() << std::endl;
      },
      "t1");
  t1->start();
  delete t1;
  t1->join();

  recipes::thread t2(std::bind(threadWithOutput, 20), "t2");
  t2.start();
  t2.join();
}