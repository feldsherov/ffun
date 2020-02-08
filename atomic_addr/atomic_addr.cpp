#include <gtest/gtest.h>

#include <atomic>

TEST(Atomic, GetAddr) {
  std::atomic<int32_t> value;
  value.store(42);
  EXPECT_EQ(*reinterpret_cast<int32_t*>(&value), 42);
}
