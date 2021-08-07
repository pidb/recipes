#include "server.h"

#include <cstdio>
#include <functional>

#include "muduo/base/Logging.h"

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

FileTransferServer::FileTransferServer(EventLoop* loop,
                                       const InetAddress& listenAddr)
    : server_(new TcpServer(loop, listenAddr, "FileTransferServer")) {
  server_->setMessageCallback(
      std::bind(&FileTransferServer::onMessage, this, _1, _2, _3));

  server_->setWriteCompleteCallback(
      std::bind(&FileTransferServer::onWriteComplete, this, _1));
}

void FileTransferServer::onMessage(const TcpConnectionPtr& conn, Buffer* buf,
                                   Timestamp ts) {
  const char* eol = buf->findEOL();
  if (!eol) {
    conn->send("file invalid");
    conn->shutdown();
    return;
  }
  std::string filename(buf->peek(), eol);

  if (!conn->connected()) {
    // FIXME lifetime binding to TcpConnection
    if (!conn->getContext().empty()) {
      FILE* fp = boost::any_cast<FILE*>(conn->getContext());
      ::fclose(fp);
    }
  } else {
    // FIXME to share fp
    FILE* fp = ::fopen(filename.c_str(), "r");
    if (!fp) {
      conn->send("FileTransferServer: no such file.");
      conn->shutdown();
      return;
    }

    char buf[kBufSize];
    size_t nread;
    if ((nread = ::fread(buf, 1, sizeof buf, fp)) > 0) {
      conn->send(buf, nread);
      conn->setContext(fp);

    } else {
      ::fclose(fp);
      conn->send("FileTransferServer: transfer done.");
      conn->shutdown();
    }
  }
}

void FileTransferServer::onWriteComplete(const TcpConnectionPtr& conn) {
  FILE* fp = boost::any_cast<FILE*>(conn->getContext());
  char buf[kBufSize];
  size_t nread;
  if ((nread = ::fread(buf, 1, sizeof buf, fp)) > 0) {
    conn->send(buf, nread);
  } else {
    ::fclose(fp);
    fp = NULL;
    conn->setContext(fp);
    conn->send("FileTransferServer: transfer done.");
    conn->shutdown();
  }
}
