enable_testing()

add_library(thread STATIC thread.h  thread.cc mutext.h)

add_subdirectory(test)

add_executable(test ThreadTest.cc)

target_link_libraries(test thread)


add_executable(
  hello_test
  hello_test.cc
)
target_link_libraries(
  hello_test
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)