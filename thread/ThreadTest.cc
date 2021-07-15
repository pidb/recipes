#include <functional>
#include <iostream>

#include "thread.h"

void threadWithOutput(int i) {
  std::cout << "id=" << recipes::this_thread::get_id()
            << ", name=" << recipes::this_thread::get_name() << " output=" << i
            << std::endl;
}

int main() {
  recipes::thread* t1 = new recipes::thread(
      [] {
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