# recipes
Some code snippets for sharing

# Table of Contents

## recipes of thread

- [deadlock for posting when traversing](thread/test/TraversePostDeadlock.cc)
- [use delay post to fix deadlock for posting when traversing](thread/test/TraversePostDeadlock_FixDelay.cc)
- [use simple of COW (copy on write) to fix deadlock for posting when traversing](thread/test/TraversePostDeadlock_FixCOW.cc)
- [deadlock: self call](thread/test/SelfDeadlock.cc)
- [fix deadlock: splitting a locked call into two parts](thread/test/SelfDeadlock_Fix.cc)
- [InventoryRequest: deadlock when hold each other's locks and wait](thread/test/InventoryRequest.cc)
- [InventoryRequest: fix deadlock with separate locked and unlocked calls to aviod holding each other's locks](thread/test/InventoryRequest_Fix.cc)
- [InventoryRequest: different call times to avoid deadlock](thread/test/InventoryRequest2.cc)
- [InventoryRequest: deadlock and race condition when destructor](thread/test/InventoryRequest3.cc)
- [InventoryRequest: fix deadlock and race condition when destructor use weak_ptr](thread/test/InventoryRequest3_Fix.cc)