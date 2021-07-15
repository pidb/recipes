#include <cassert>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

class BankAccount {
 public:
  BankAccount() = default;
  explicit BankAccount(std::string name, int64_t money)
      : name_(name), money_(money) {}
  void transfer(BankAccount& other, size_t amount);
  int16_t get_money() const { return money_; }

 private:
  int64_t money_;
  std::string name_;
  std::mutex mutex_;
};

void BankAccount::transfer(BankAccount& to, size_t amount) {
  std::unique_lock<std::mutex> self_lock_(
      mutex_, std::defer_lock);  // tell unique_lock to delay lock
  std::unique_lock<std::mutex> other_lock_(to.mutex_, std::defer_lock);
  std::lock(self_lock_, other_lock_);  // avoid deadlock
  money_ -= amount;
  to.money_ += amount;
}

void transfer(BankAccount& from, BankAccount& to, size_t amount) {
  from.transfer(to, amount);
}

int main() {
  BankAccount foo("foo", 100);
  BankAccount bar("bar", 200);

  std::thread t1([&foo, &bar] { foo.transfer(bar, 50); });

  std::thread t2([&foo, &bar] { bar.transfer(foo, 30); });

  t1.join();
  t2.join();
  assert(foo.get_money() == 80 && bar.get_money() == 220);
}