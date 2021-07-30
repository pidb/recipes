#include <algorithm>
#include <functional>
#include <iostream>
#include <set>
#include <vector>

typedef std::function<void()> EventCallback;

class Channel {
 public:
  void HandleEvent();
  void SetReadCallback(const EventCallback& cb);

 private:
  EventCallback readCallback_;
};

void Channel::HandleEvent() { readCallback_(); }

void Channel::SetReadCallback(const EventCallback& cb) { readCallback_ = cb; }

class ChannelManager {
 public:
  void Register(Channel* channel) { channels_.insert(channel); }
  void UnRegister(Channel* channel) { channels_.erase(channel); }
  void Run(std::vector<Channel*>* channelList) {
    std::for_each(
        channels_.begin(), channels_.end(),
        [&channelList](Channel* channel) { channelList->push_back(channel); });
    for (auto it = channelList->begin(); it != channelList->end(); ++it) {
      (*it)->HandleEvent();
    }
  }

 private:
  std::set<Channel*> channels_;
};

class TcpConnection {
 public:
  TcpConnection(ChannelManager* channelManager);

 private:
  void HandleRead();

  Channel* channel_;
  ChannelManager* channelManager_;
};

TcpConnection::TcpConnection(ChannelManager* channelManager)
    : channel_(new Channel), channelManager_(channelManager) {
  channel_->SetReadCallback(std::bind(&TcpConnection::HandleRead, this));
  channelManager_->Register(channel_);
}

void TcpConnection::HandleRead() {
  std::cout << "call handleRead" << std::endl;
  channelManager_->UnRegister(channel_);
}

int main() {
  ChannelManager channelManager;
  std::vector<Channel*> activeChannels;
  TcpConnection tcpConn1(&channelManager);
  TcpConnection tcpConn2(&channelManager);
  channelManager.Run(&activeChannels);
}