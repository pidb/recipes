enable_testing()

add_executable(
  basic
  basic.cc
)
target_link_libraries(
  basic
  gtest
  gtest_main
  pthread
)
