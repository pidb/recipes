#include "muduo/base/Timestamp.h";
#include "muduo/net/Buffer.h"
#include "muduo/net/TcpConnection.h"
#include "muduo/net/TcpServer.h"

using muduo::Timestamp;
using muduo::net::Buffer;
using muduo::net::EventLoop;
using muduo::net::InetAddress;
using muduo::net::TcpConnectionPtr;
using muduo::net::TcpServer;
class EchoServer {
 public:
  EchoServer(EventLoop* loop, const InetAddress& listenAddr);

  void Start();

  void OnConnection(const TcpConnectionPtr& conn);

  void OnMessage(const TcpConnectionPtr& conn, Buffer* buf, Timestamp ts);

 private:
  TcpServer* server_;
};