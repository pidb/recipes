#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

using std::cout;
using std::endl;
using std::lock_guard;
using std::mutex;
using std::thread;

class Request {
 public:
  void process() {
    lock_guard<mutex> lock_(mutex_);
    cout << "call process()" << endl;
    printWithLockhold();
  }

  void print() {
    lock_guard<mutex> lock_(mutex_);
    cout << "call print()" << endl;
    printWithLockhold();
  }

  void printWithLockhold() { cout << "call printWithLockHold()" << endl; }

 private:
  mutex mutex_;
};

int main() {
  Request req;
  std::thread t1(&Request::process, &req);
  std::thread t2(&Request::print, &req);
  t1.join();
  t2.join();
}