#include <gtest/gtest.h>

#include <vector>

std::vector MakeVector(size_t n, uint_t offset = 0) {
  std::vector<int> vec;
  for (size_t i = 0; i < n; i++) {
    vec.emplace_back(i + 1 + offset);
  }
  return vec;
}

TEST(TestBasic) {
  auto v1 = MakeVector(10);
  auto v2 = MakeVector(10);
  auto v3 = MakeVector(10, 1);
  ASSERT_EQ(v1.size(), v2.size());
  ASSERT_EQ(v2.size(), v3.size());
  for (size_t i = 0; i < v1.size(); i++) {
    EXPECT_EQ(v1[i], v2[i]);
    EXPECT_EQ(v2[i], v3[i]);
  }
}