COMPILER=g++
CXXFLAGS=-g -Wall -std=c++17
CXXFLAGS+=-O2
CLEAN_OTHER=

HEADERS=$(wildcard *.h)
TESTS= traversepostdeadlock_test \
       traversepostdeadlock_fixdelay_test \
	   traversePostDeadlock_fixcow_test \
	   selfdeadlock_test \
	   selfdeadlock_fix_test \
	   inventoryrequest_test \
	   inventoryrequest_test2 \
	   inventoryrequest_test3 \
	   inventoryrequest_fix_test \
	   inventoryrequest_fix_test3 \
	   simpleblockingqueue_test \
	   countdownlatch_test \

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S), Linux)
	CXXFLAGS += -D Linux -lpthread -march=native -rdynamic
endif

ifeq ($(UNAME_S), Darwin)
	CXXFLAGS += -D OSX -stdlib=libc++
	COMPILER=clang++
	CLEAN_OTHER += *.dSYM
endif	


all: $(TESTS)

$(TESTS): $(HEADERS)

$(TESTS):
	${COMPILER} $(CXXFLAGS) -o $@ $(filter %.cc,$^) $(LDFLAGS)

traversepostdeadlock_test: test/TraversePostDeadlock.cc

traversepostdeadlock_fixdelay_test: test/TraversePostDeadlock_FixDelay.cc

traversePostDeadlock_fixcow_test: test/TraversePostDeadlock_FixCOW.cc

selfdeadlock_test: test/SelfDeadlock.cc 

selfdeadlock_fix_test: test/SelfDeadlock_Fix.cc

inventoryrequest_test: test/InventoryRequest.cc

inventoryrequest_test2: test/InventoryRequest2.cc

inventoryrequest_test3: test/InventoryRequest3.cc

inventoryrequest_fix_test: test/InventoryRequest_Fix.cc

inventoryrequest_fix_test3: test/InventoryRequest3_Fix.cc

simpleblockingqueue_test: test/SimpleBlockingQueue.cc

countdownlatch_test: test/CountDownLatch.cc

clean:
	rm -rf $(TESTS) $(CLEAN_OTHER)
