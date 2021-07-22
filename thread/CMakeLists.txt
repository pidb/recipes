add_library(thread STATIC thread.h  thread.cc mutext.h)

add_subdirectory(test)

add_executable(test ThreadTest.cc)

target_link_libraries(test thread)