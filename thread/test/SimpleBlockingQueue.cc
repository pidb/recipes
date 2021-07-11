#include <cassert>
#include <chrono>
#include <condition_variable>
#include <ctime>
#include <deque>
#include <iostream>
#include <mutex>
#include <thread>

using std::condition_variable;
using std::deque;
using std::lock_guard;
using std::mutex;
using std::thread;
using std::unique_lock;

template <typename T>
class BlockingQueue {
 public:
  BlockingQueue() = default;
  explicit BlockingQueue(size_t n) : q_(deque<T>(n)){};
  void enqueue(T data);
  T dequeue();
  bool empty() const { q_.empty(); }
  size_t size() const { q_.size(); }

 private:
  mutex mutex_;
  condition_variable cv_;
  deque<T> q_;
};

template <typename T>
void BlockingQueue<T>::enqueue(T data) {
  lock_guard<mutex> lock_(mutex_);
  q_.template emplace_back(data);
  cv_.notify_one();
}

template <typename T>
T BlockingQueue<T>::dequeue() {
  unique_lock<mutex> lock_(mutex_);
  while (q_.empty()) {
    cv_.wait(lock_);
  }
  assert(!q_.empty());
  T top = q_.front();
  q_.pop_front();
  return top;
}

class Entity {
 public:
  Entity() : id_(0){};
  explicit Entity(int id) : id_(id) {}
  int get_id() const { return id_; }

 private:
  int id_;
};

std::ostream& operator<<(std::ostream& out, const Entity& entity) {
  out << "Entity " << entity.get_id();
  return out;
}

int main() {
  BlockingQueue<Entity> blockQ;
  thread work1([&blockQ]() {
    for (int i = 0; i < 10; i++) {
      Entity entity = blockQ.dequeue();
      auto now = std::chrono::system_clock::to_time_t(
          std::chrono::system_clock::now());
      std::cout << std::this_thread::get_id() << ": got " << entity << " in "
                << std::ctime(&now);
    }
  });

  thread work2([&blockQ]() {
    for (int i = 0; i < 10; i++) {
      Entity entity = blockQ.dequeue();
      auto now = std::chrono::system_clock::to_time_t(
          std::chrono::system_clock::now());
      std::cout << std::this_thread::get_id() << ": got " << entity << " in "
                << std::ctime(&now);
    }
  });

  thread master1([&blockQ]() {
    for (int i = 0; i < 20; i++) {
      std::this_thread::sleep_for(std::chrono::milliseconds(i * 200));
      blockQ.enqueue(Entity(i + 1));
    }
  });

  work1.join();
  work2.join();
  master1.join();
  return 0;
}