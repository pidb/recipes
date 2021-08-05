#ifndef RECIPES_NET_BUFFER_H
#define RECIPES_NET_BUFFER_H

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace recipes {
namespace net {
// FIXME move to Buffer class
static const size_t InitializerSize = 1024;
static const size_t PrependableSize = 8;
class Buffer {
 public:
 public:
  Buffer()
      : read_index_(PrependableSize),
        write_index_(PrependableSize),
        bytes_(ByteList(PrependableSize + InitializerSize)) {}

  ~Buffer() {}

  size_t prependable_bytes() const { return read_index_; }

  size_t readable_bytes() const { return write_index_ - read_index_; }

  size_t writeable_bytes() const { return bytes_.size() - write_index_; }

  std::string read_string(size_t len) {
    assert(len > 0 && len < bytes_.size() && len <= readable_bytes());

    std::string result(peek(), len);  // should be moveable
    retrieve(len);
    return result;
  }

  // TODO test me
  void read_bytes(char* bytes, size_t len) {
    assert(len > 0 && len < bytes_.size() && len <= readable_bytes());
    std::copy(peek(), peek() + len, bytes);
    retrieve(len);
  }

  void retrieve(size_t len) { read_index_ += len; }

  const char* peek() { return first() + read_index_; }

  void append(const char* bytes, size_t len) noexcept {
    // FIXME try_grow
    std::copy(bytes, bytes + len, first_write());
    write_index_ += len;
  }

 private:
  // first is from vector begin iterator
  char* first() { return &*bytes_.begin(); }
  // first_write is from write_index_
  char* first_write() { return &*(bytes_.begin() + write_index_); }
  typedef std::vector<char> ByteList;
  size_t read_index_;
  size_t write_index_;
  ByteList bytes_;
};
}  // namespace net
}  // namespace recipes

#endif /* RECIPES_NET_BUFFER_H */