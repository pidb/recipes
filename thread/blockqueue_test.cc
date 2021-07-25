#include "blockqueue.h"

#include <gtest/gtest.h>

#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "thread.h"

class BlockQueueTest : public ::testing::Test {
 protected:
  BlockQueueTest();
  ~BlockQueueTest() = default;
  void runProducer();
  void runConsumer();
  void consumerFn();
  void producerFn();
  void joinAll();

  size_t consumerThreadNums_;
  int exceptValue_;
  std::unique_ptr<recipes::thread> producerThread_;
  std::vector<std::unique_ptr<recipes::thread>> consumerThreadManager_;
  std::vector<bool> collector_;
  recipes::BlockQueue<std::string> blockQ_;
};

BlockQueueTest::BlockQueueTest() {
  exceptValue_ = 100;
  consumerThreadNums_ = 5;
  collector_ = std::vector<bool>(exceptValue_, false);
  for (int i = 0; i < exceptValue_; i++) {
    blockQ_.add(std::to_string(i));
  }
}

void BlockQueueTest::consumerFn() {
  while (true) {
    std::string s = blockQ_.take();
    if (s == "stop") {
      break;
    }
    collector_[atoi(s.c_str())] = true;
  }
}

void BlockQueueTest::runConsumer() {
  for (size_t i = 0; i < consumerThreadNums_; i++) {
    consumerThreadManager_.emplace_back(new recipes::thread(
        std::bind(&BlockQueueTest::consumerFn, this), "thread" + i));
  }

  for (size_t i = 0; i < consumerThreadNums_; i++) {
    consumerThreadManager_[i]->start();
  }
}

void BlockQueueTest::joinAll() {
  for (size_t i = 0; i < consumerThreadNums_; i++) {
    consumerThreadManager_[i]->join();
  }
  producerThread_->join();
}

void BlockQueueTest::producerFn() {
  for (int i = 0; i < consumerThreadNums_; i++) {
    blockQ_.add("stop");
  }
}

void BlockQueueTest::runProducer() {
  producerThread_ = std::unique_ptr<recipes::thread>(new recipes::thread(
      std::bind(&BlockQueueTest::producerFn, this), "producer"));
  producerThread_->start();
}

TEST_F(BlockQueueTest, Basic) {
  runConsumer();
  runProducer();
  joinAll();
  for (size_t i = 0; i < collector_.size(); i++) {
    EXPECT_TRUE(collector_[i]) << i;
  }
}
