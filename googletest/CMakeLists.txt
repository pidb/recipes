enable_testing()

add_executable(
  basic
  basic.cc
)
target_link_libraries(
  basic
  gtest_main
)

include(GoogleTest)

gtest_discover_tests(basic)