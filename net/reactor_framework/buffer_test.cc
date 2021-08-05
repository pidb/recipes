#include "buffer.h"

#include <gtest/gtest.h>

#include <iostream>
#include <string>

using recipes::net::Buffer;
using std::string;

TEST(BufferTest, BufferDefaultInitializer) {
  Buffer buffer;
  ASSERT_EQ(buffer.readable_bytes(), 0);
  ASSERT_EQ(buffer.writeable_bytes(), recipes::net::InitializerSize);
  ASSERT_EQ(buffer.prependable_bytes(), recipes::net::PrependableSize);
}

TEST(BufferTest, BufferOperators) {
  Buffer buffer;

  // write 200 bytes
  string packet1(200, 'x');
  buffer.append(packet1.c_str(), 200);
  ASSERT_EQ(buffer.prependable_bytes(), recipes::net::PrependableSize);
  ASSERT_EQ(buffer.readable_bytes(), packet1.size());
  ASSERT_EQ(buffer.writeable_bytes(),
            recipes::net::InitializerSize - packet1.size());

  // readout 50 bytes
  string str = buffer.read_string(50);
  ASSERT_EQ(buffer.prependable_bytes(),
            recipes::net::PrependableSize + str.size());
  ASSERT_EQ(buffer.readable_bytes(), packet1.size() - str.size());
  ASSERT_EQ(buffer.writeable_bytes(),
            recipes::net::InitializerSize - packet1.size());
  ASSERT_EQ(str, string(50, 'x'));

  // write 200 bytes;
  string packet2(200, 'y');
  buffer.append(packet2.c_str(), 200);
  ASSERT_EQ(buffer.readable_bytes(), packet1.size() + packet2.size() - 50);
  ASSERT_EQ(buffer.writeable_bytes(),
            recipes::net::InitializerSize - (packet1.size() + packet2.size()));

  // readout 350 bytes;200
  string str2 = buffer.read_string(350);
  ASSERT_EQ(buffer.prependable_bytes(),
            recipes::net::PrependableSize + str.size() + str2.size());
  ASSERT_EQ(buffer.readable_bytes(),
            packet1.size() + packet2.size() - str.size() - str2.size());
  ASSERT_EQ(buffer.writeable_bytes(),
            recipes::net::InitializerSize - (packet1.size() + packet2.size()));
  ASSERT_EQ(str2, string(150, 'x') + string(200, 'y'));
}