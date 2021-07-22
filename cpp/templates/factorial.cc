#include <iostream>

template <int n>
struct factorial {
  static_assert(n >= 0, "arg must non-negative");
  static const int value = n * factorial<n - 1>::value;
};

template <>
struct factorial<0> {
  static const int value = 1;
};

int main() { std::cout << factorial<10>::value << std::endl; }
