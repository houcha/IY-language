#include <iostream>
#include "stack.hpp"
#include "gtest/gtest.h"

struct UserStruct {
  int  i;
  char ch;
  std::vector<int> vec;
  bool b;
};

template<>
UserStruct Poison<UserStruct>() {
  return { 11, '\0', {-1, -1}, true};
}

//========================= DYNAMIC STACK TESTS =================================
TEST(DynamicStackTest, TestCreation) {

  DynamicStack<int> stack0;
  DynamicStack<uint64_t> stack1;
  DynamicStack<double> stack2;
  DynamicStack<bool> stack3;
  DynamicStack<UserStruct> stack4;
}

TEST(DynamicStackTest, TestPushAndPop) {

  DynamicStack<int> stack;

  // Check init state.
  EXPECT_EQ(stack.empty(), true);
  EXPECT_EQ(stack.size(), 0);

  stack.pop();
  EXPECT_EQ(stack.empty(), true);
  EXPECT_EQ(stack.size(), 0);

  stack.push(0);
  // 0
  EXPECT_EQ(stack.top(), 0);
  EXPECT_EQ(stack.empty(), false);
  EXPECT_EQ(stack.size(), 1);

  stack.push(1234567);
  // 0 1234567
  EXPECT_EQ(stack.top(), 1234567);
  EXPECT_EQ(stack.empty(), false);
  EXPECT_EQ(stack.size(), 2);

  stack.push(-321);
  // 0 1234567 -321
  EXPECT_EQ(stack.top(), -321);
  EXPECT_EQ(stack.empty(), false);
  EXPECT_EQ(stack.size(), 3);

  stack.pop();
  // 0 1234567
  EXPECT_EQ(stack.top(), 1234567);
  EXPECT_EQ(stack.empty(), false);
  EXPECT_EQ(stack.size(), 2);

  stack.push(999);
  // 0 1234567 999
  EXPECT_EQ(stack.top(), 999);
  EXPECT_EQ(stack.empty(), false);
  EXPECT_EQ(stack.size(), 3);

  stack.push(999);
  // 0 1234567 999 999
  EXPECT_EQ(stack.top(), 999);
  EXPECT_EQ(stack.empty(), false);
  EXPECT_EQ(stack.size(), 4);

  stack.pop();
  // 0 1234567 999
  EXPECT_EQ(stack.top(), 999);
  EXPECT_EQ(stack.empty(), false);
  EXPECT_EQ(stack.size(), 3);

  stack.pop();
  // 0 1234567
  EXPECT_EQ(stack.top(), 1234567);
  EXPECT_EQ(stack.empty(), false);
  EXPECT_EQ(stack.size(), 2);

  stack.pop();
  // 0
  EXPECT_EQ(stack.top(), 0);
  EXPECT_EQ(stack.empty(), false);
  EXPECT_EQ(stack.size(), 1);

  stack.pop();
  EXPECT_EQ(stack.empty(), true);
  EXPECT_EQ(stack.size(), 0);
}

//========================= STATIC STACK TESTS =================================
TEST(StaticStackTest, TestCreationOfInt) {

  // 0 is incorrect size.
  StaticStack<int, 1> stack0;
  StaticStack<int, 10> stack1;
  StaticStack<int, 100> stack2;
  StaticStack<int, 1000> stack3;
  StaticStack<int, 10000> stack4;
  StaticStack<int, 100000> stack5;
  StaticStack<int, 1000000> stack6;
  // 10000000 capacity causes overflow.
}

TEST(StaticStackTest, TestCreationOfUserStruct) {

  StaticStack<UserStruct, 1> stack0;
  StaticStack<UserStruct, 10> stack1;
  StaticStack<UserStruct, 100> stack2;
  StaticStack<UserStruct, 1000> stack3;
  StaticStack<UserStruct, 10000> stack4;
  StaticStack<UserStruct, 100000> stack5;
  // Bigger capacity causes overflow.
}

TEST(StaticStackTest, TestPushInt) {

  const int size = 2;

  StaticStack<int, size> stack;

  // Check init state.
  EXPECT_EQ(stack.empty(), true);
  EXPECT_EQ(stack.size(), 0);
  EXPECT_EQ(stack.capacity(), 2);

  stack.pop();
  EXPECT_EQ(stack.empty(), true);
  EXPECT_EQ(stack.size(), 0);
  EXPECT_EQ(stack.capacity(), 2);

  stack.push(0);
  // 0
  EXPECT_EQ(stack.top(), 0);
  EXPECT_EQ(stack.empty(), false);
  EXPECT_EQ(stack.size(), 1);
  EXPECT_EQ(stack.capacity(), 2);

  stack.push(1234567);
  // 0 1234567
  EXPECT_EQ(stack.top(), 1234567);
  EXPECT_EQ(stack.empty(), false);
  EXPECT_EQ(stack.size(), 2);
  EXPECT_EQ(stack.capacity(), 2);

  stack.push(-321);
  // 0 1234567 -321
  EXPECT_EQ(stack.top(), -321);
  EXPECT_EQ(stack.empty(), false);
  EXPECT_EQ(stack.size(), 3);
  EXPECT_GT(stack.capacity(), 2);

  stack.pop();
  // 0 1234567
  EXPECT_EQ(stack.top(), 1234567);
  EXPECT_EQ(stack.empty(), false);
  EXPECT_EQ(stack.size(), 2);

  stack.push(999);
  // 0 1234567 999
  EXPECT_EQ(stack.top(), 999);
  EXPECT_EQ(stack.empty(), false);
  EXPECT_EQ(stack.size(), 3);
  EXPECT_GT(stack.capacity(), 2);

  stack.push(999);
  // 0 1234567 999 999
  EXPECT_EQ(stack.top(), 999);
  EXPECT_EQ(stack.empty(), false);
  EXPECT_EQ(stack.size(), 4);
  EXPECT_GT(stack.capacity(), 2);

  stack.pop();
  // 0 1234567 999
  EXPECT_EQ(stack.top(), 999);
  EXPECT_EQ(stack.empty(), false);
  EXPECT_EQ(stack.size(), 3);
  EXPECT_GT(stack.capacity(), 2);

  stack.pop();
  // 0 1234567
  EXPECT_EQ(stack.top(), 1234567);
  EXPECT_EQ(stack.empty(), false);
  EXPECT_EQ(stack.size(), 2);

  stack.pop();
  // 0
  EXPECT_EQ(stack.top(), 0);
  EXPECT_EQ(stack.empty(), false);
  EXPECT_EQ(stack.size(), 1);

  stack.pop();
  //
  EXPECT_EQ(stack.empty(), true);
  EXPECT_EQ(stack.size(), 0);
}


int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

