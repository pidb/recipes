#include <unistd.h>

#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <set>
#include <thread>

using std::cout;
using std::endl;
using std::lock_guard;
using std::mutex;

class Inventory;
class Request;

typedef std::set<Request*> RequestSet;

class Inventory {
 public:
  void add(Request* req) {
    lock_guard<mutex> lock_(mutex_);
    requests_.insert(req);
  }

  void del(Request* req) {
    lock_guard<mutex> lock_(mutex_);
    cout << "call Inventory::del" << endl;
    requests_.erase(req);
  }

  void printAll();

 private:
  RequestSet requests_;
  mutex mutex_;
};

class Request {
 public:
  explicit Request(Inventory* inventory) : inventory_(inventory) {}
  void process() {
    lock_guard<mutex> lock_(mutex_);
    inventory_->add(this);
  }

  void print() {
    lock_guard<mutex> lock_(mutex_);
    cout << "call Request::print" << endl;
  }
  ~Request() {
    lock_guard<mutex> lock_(mutex_);
    std::this_thread::sleep_for(std::chrono::seconds(1));  // sleep 1s
    cout << "call Request::~Request" << endl;

    inventory_->del(this);
  }

 private:
  mutex mutex_;
  Inventory* inventory_;
};

void Inventory::printAll() {
  std::this_thread::sleep_for(std::chrono::seconds(1));  // sleep 1s
  lock_guard<mutex> lock_(mutex_);
  cout << "call Inventory::printAll" << endl;
  for (auto it = requests_.begin(); it != requests_.end(); it++) {
    (*it)->print();
  }
}

/*
Request::~Request
  -> Inventory::del
                    Inventory::printAll
                      -> Request::print
*/

void func(Inventory* inventory) {
  Request* req = new Request(inventory);
  req->process();
  delete req;
}

int main() {
  Inventory inventory;
  std::thread t1(func, &inventory);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));  // sleep 500ms
  inventory.printAll();
  t1.join();
  return 0;
}
