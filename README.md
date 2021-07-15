# recipes
Some code snippets for sharing

# Table of Contents

## recipes of thread

- [TraversePostDeadlock: 展示了某个线程 `push_back` 而另一线程 `traverse` 死锁的情况](thread/test/TraversePostDeadlock.cc)
- [TraversePostDeadlock: 一种延迟修改的思路避免某个线程 `push_back` 而另一线程 `traverse` 造成迭代器失效的情况](thread/test/TraversePostDeadlock_FixDelay.cc)
- [TraversePostDeadlock: 使用 `shared_ptr` 的机制来完成简单的写时复制以避免多线程迭代器失效的情况](thread/test/TraversePostDeadlock_FixCOW.cc)
- [SelfDeadlock: 展示了同一对象自己的多个有锁函数多次调用死锁的情况](thread/test/SelfDeadlock.cc)
- [SelfDeadlock: 分离有锁和无锁调用避免产生不可重入锁多次调用的情况](thread/test/SelfDeadlock_Fix.cc)
- [InventoryRequest: 展示了互相持有对方锁并等待造成的死锁情况](thread/test/InventoryRequest.cc)
- [InventoryRequest: 分离有锁和无锁调用避免调用产生互相持有对方锁并等待造成的死锁情况](thread/test/InventoryRequest_Fix.cc)
- [InventoryRequest: 展示了即使互相持有对方锁但时间不同也可以避免死锁](thread/test/InventoryRequest2.cc)
- [InventoryRequest: 展示了对象正在析构而另一个函数正在调用它的成员](thread/test/InventoryRequest3.cc)
- [InventoryRequest: 使用 `weak_ptr` 弱指针修复对象在析构函数时另一线程调用了这个对象的问题](thread/test/InventoryRequest3_Fix.cc)
- [SimpleBlockingQueue: 使用 `condition_variable` `mutex` 实现的一个简单的阻塞队列](thread/test/SimpleBlockingQueue.cc)
- [CountDownLatch: 使用 `condition_variable` `mutex` 实现的一个线程倒计时, 类似 Golang 的 `sync.WaitGroup`](thread/test/CountDownLatch.cc)
- [BackTransfer: 一个使用 `unique_lock` 的银行转账示例]