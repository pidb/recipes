#include <cstdio>

#include "muduo/base/Timestamp.h"
#include "muduo/net/TcpServer.h"

using muduo::Timestamp;
using muduo::net::Buffer;
using muduo::net::EventLoop;
using muduo::net::InetAddress;
using muduo::net::TcpConnectionPtr;
using muduo::net::TcpServer;

const size_t kBufSize = 64 * 1024;

class FileTransferServer {
 public:
  FileTransferServer(EventLoop* loop, const InetAddress& listenAddr);

  void onMessage(const TcpConnectionPtr& ptr, Buffer* buf, Timestamp ts);

  void onWriteComplete(const TcpConnectionPtr& conn);

  void start() { server_->start(); }

 private:
  TcpServer* server_;
};