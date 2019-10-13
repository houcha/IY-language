#include <iostream>
#include "stack.hpp"
#include "gtest/gtest.h"

TEST(StaticStackTest, StressTest) {

  StaticStack<int, 100> stack;

  size_t N = 10000000000;
  for (int i = 0; i < N; ++i) {
    stack.push(i);
    EXPECT_EQ(stack.top(), i);
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

