#include "CountDownLatch.h"

#include <gtest/gtest.h>

#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "thread.h"

// recipes::MutexLock done_orders_mutex;
// std::vector<std::string> done_orders;

struct collector {
 public:
  void push_back(std::string threadName) {
    recipes::MutexGuard lock_(done_orders_mutex);
    done_orders.push_back(threadName);
  }
  void check() {
    std::cout << "size=" << done_orders.size() << std::endl;
    for (const auto& thName : done_orders) {
      std::cout << thName << std::endl;
    }
  }

 private:
  recipes::MutexLock done_orders_mutex;
  std::vector<std::string> done_orders;
};

TEST(TestCountDownLatch, Basic) {
  int threadGroup1 = 5, threadGroup2 = 2;
  std::vector<std::unique_ptr<recipes::thread>> tg1, tg2;
  recipes::CountDownLatch barrier1(threadGroup1), barrier2(threadGroup2);
  collector collect;
  for (int i = 0; i < threadGroup1; i++) {
    tg1.emplace_back(new recipes::thread(
        [&barrier1, &barrier2, &collect] {
          barrier1.Done();
          barrier2.Wait();
          collect.push_back(recipes::this_thread::get_name());
        },
        "g1"));
    tg1[i]->start();
  }

  barrier1.Wait();
  for (int i = 0; i < threadGroup2; i++) {
    tg2.emplace_back(new recipes::thread(
        [&barrier2, &collect] {
          barrier2.Done();
          collect.push_back(recipes::this_thread::get_name());
        },
        "g2"));
    tg2[i]->start();
  }

  barrier2.Wait();
  for (int i = 0; i < threadGroup2; i++) {
    tg1[i]->join();
    tg2[i]->join();
  }

  collect.check();
}