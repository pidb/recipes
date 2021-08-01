enable_testing()

add_executable(
  buffer_test
  buffer.h
  buffer_test.cc
)
target_link_libraries(
  buffer_test
  gtest
  gtest_main
  pthread
)
