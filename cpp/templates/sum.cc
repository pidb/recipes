#include <iostream>

template <typename T, T v>
struct IntegralCompiler {
  static const T value = v;
};

template <int result, int n>
struct SumLoop {
  static const bool cond_value = n != 0;
  static const int res_value = result;
  typedef IntegralCompiler<int, res_value> res_type;
  typedef SumLoop<res_value + n, n - 1> next_type;
};

template <int n>
struct SumBody {
  typedef SumLoop<0, n> type;
};

template <bool cond_value, typename Body>
struct WhileLoop;

template <typename Body>
struct WhileLoop<true, Body> {
  typedef
      typename WhileLoop<Body::cond_value, typename Body::next_type>::type type;
};

template <typename Body>
struct WhileLoop<false, Body> {
  typedef typename Body::res_type type;
};

template <typename Body>
struct While {
  typedef typename WhileLoop<Body::cond_value, Body>::type type;
};

int main() { std::cout << While<SumBody<10>::type>::type::value << std::endl; }
