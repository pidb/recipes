enable_testing()

set (RECIPES_SRCS 
  thread.h  
  mutext.h 
  condition.h 
  blockqueue.h 
  CountDownLatch.h
  thread.cc 
)

add_library(recipes_base ${RECIPES_SRCS})

target_link_libraries(recipes_base pthread)


# add_library(recipesthread STATIC 

  
#    )

add_executable(
  blockqueue_test
  blockqueue_test.cc
)

target_link_libraries(
  blockqueue_test
  gtest
  gtest_main
  recipes_base
)


add_executable(
  countdownlatch_test
  countdownlatch_test.cc
)

target_link_libraries(
  countdownlatch_test
  gtest
  gtest_main
  recipes_base
)


add_test(CountDownLatchTest countdownlatch_test)

add_subdirectory(test)

# add_executable(test ThreadTest.cc)

