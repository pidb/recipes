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

bool operator<(const Request& lhs, const Request& rhs) { return &lhs < &rhs; }

struct lex_compare {
  bool operator()(const std::weak_ptr<Request>& lhs,
                  const std::weak_ptr<Request>& rhs) const {
    auto lptr = lhs.lock(), rptr = rhs.lock();
    if (!rptr) return false;  // nothing after expired pointer
    if (!lptr) return true;   // every not expired after expired pointer
    return *lptr < *rptr;
  }
};

typedef std::set<std::weak_ptr<Request>, lex_compare> RequestSet;

class Inventory {
 public:
  void add(std::weak_ptr<Request> req) {
    lock_guard<mutex> lock_(mutex_);
    cout << "call Inventory::add" << endl;
    requests_.insert(req);
  }

  void del(std::weak_ptr<Request> req) {
    lock_guard<mutex> lock_(mutex_);
    cout << "call Inventory::del" << endl;
    requests_.erase(req);
  }

  void printAll();

 private:
  RequestSet requests_;
  mutex mutex_;
};

Inventory g_inventory_;

class Request : public std::enable_shared_from_this<Request> {
 public:
  Request() = default;
  void process() {
    lock_guard<mutex> lock_(mutex_);
    g_inventory_.add(shared_from_this());
  }

  void print() { lock_guard<mutex> lock_(mutex_); }

  ~Request() {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));  // sleep 1s
    lock_guard<mutex> lock_(mutex_);
    cout << "call Request::~Request" << endl;
  }

 private:
  mutex mutex_;
};

void Inventory::printAll() {
  // lock_guard<mutex> lock_(mutex_);
  cout << "call Inventory::printAll" << endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(500));  // sleep 500ms
  auto it = requests_.begin();
  while (it != requests_.end()) {
    std::shared_ptr<Request> up(it->lock());
    if (up) {
      up->print();
      ++it;  // iterator valid only here
    } else {
      cout << "Request invliad erase it." << endl;
      it = requests_.erase(it);
    }
  }
}
/*
                    Request::~Request
                                         Inventory::del
Inventory::printAll
                                      Request::print
*/

void func(Inventory* inventory) {
  std::shared_ptr<Request> req(new Request);
  req->process();
}

int main() {
  std::thread t1(func, &g_inventory_);
  std::this_thread::sleep_for(std::chrono::milliseconds(500));
  g_inventory_.printAll();
  t1.join();
  return 0;
}
