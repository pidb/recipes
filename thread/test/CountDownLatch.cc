#include <cassert>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using std::condition_variable;
using std::mutex;

class CountDownLatch {
 public:
  CountDownLatch() = default;
  explicit CountDownLatch(size_t count) : count_(count) {}
  void wait() noexcept;
  void count_down();
  size_t get_count() const { return count_; };

 private:
  size_t count_{0};
  mutex mutex_;
  condition_variable cv_;
};

void CountDownLatch::wait() noexcept {
  assert(count_ > 0);
  std::unique_lock<mutex> lock_(mutex_);
  cv_.wait(lock_, [this] { return count_ == 0; });
  assert(count_ == 0);
}

void CountDownLatch::count_down() {
  std::lock_guard<mutex> lock_(mutex_);
  count_--;
  if (count_ == 0) {
    cv_.notify_all();
  }
}

int main() {
  mutex out_mutex_;
  unsigned threads_num = std::thread::hardware_concurrency();
  CountDownLatch cdl(threads_num);
  std::vector<std::thread> works;
  works.reserve(threads_num);
  size_t wait_ms = 100;
  for (int i = 0; i < threads_num; i++) {
    works.emplace_back(
        [&cdl, wait_ms](int id) {
          std::this_thread::sleep_for(std::chrono::milliseconds(wait_ms));
          cdl.count_down();
        },
        i);
  }

  for (auto &work : works) {
    work.detach();
  }
  std::cout << "main thread wait " << threads_num << " threads done."
            << std::endl;
  auto start = std::chrono::system_clock::now();
  cdl.wait();
  auto end = std::chrono::system_clock::now();

  auto elapsed_ms =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
          .count();

  double err_range = static_cast<double>(wait_ms) * 0.099;
  assert(static_cast<double>(elapsed_ms) < (wait_ms + err_range));

  std::cout << "all " << elapsed_ms << "ms"
            << " done." << std::endl;

  assert(cdl.get_count() == 0);
  return 0;
}
