#include <gtest/gtest.h>

#include <iostream>
#include <sstream>

#include "Input.hpp"

class InputFloorCountTest : public ::testing::Test {
protected:
  std::istringstream input;
  std::ostringstream output;

  std::streambuf *cin_buffer{};
  std::streambuf *cout_buffer{};

  void SetUp() override {
    cin_buffer = std::cin.rdbuf(input.rdbuf());
    cout_buffer = std::cout.rdbuf(output.rdbuf());
  }

  void TearDown() override {
    std::cin.clear();
    std::cin.rdbuf(cin_buffer);
    std::cout.rdbuf(cout_buffer);
  }
};

TEST_F(InputFloorCountTest, AcceptsMinimumFloorCount) {
  // ARRANGE
  input.str(std::to_string(MIN_COUNT_FLOORS) + "\n");

  // ACT
  const auto result = Input::floor_count();

  // ASSERT
  EXPECT_EQ(result, MIN_COUNT_FLOORS);
}

TEST_F(InputFloorCountTest, AcceptsMaximumFloorCount) {
  // ARRANGE
  input.str(std::to_string(MAX_COUNT_FLOORS) + "\n");

  // ACT
  const auto result = Input::floor_count();

  // ASSERT
  EXPECT_EQ(result, MAX_COUNT_FLOORS);
}

TEST_F(InputFloorCountTest, RejectsTooSmallValueAndAcceptsNextInput) {
  // ARRANGE
  input.str(std::to_string(MIN_COUNT_FLOORS - 1) + "\n5\n");

  // ACT
  const auto result = Input::floor_count();

  // ASSERT
  EXPECT_EQ(result, 5);
}

TEST_F(InputFloorCountTest, RejectsTooLargeValueAndAcceptsNextInput) {
  // ARRANGE
  input.str(std::to_string(MAX_COUNT_FLOORS + 1) + "\n5\n");

  // ACT
  const auto result = Input::floor_count();

  // ASSERT
  EXPECT_EQ(result, 5);
}

TEST_F(InputFloorCountTest, RejectsInvalidInputAndAcceptsNextInput) {
  // ARRANGE
  input.str("abc\n5\n");

  // ACT
  const auto result = Input::floor_count();

  // ASSERT
  EXPECT_EQ(result, 5);
}

TEST_F(InputFloorCountTest, RejectsIntegerOverflowAndAcceptsNextInput) {
  // ARRANGE
  input.str("999999999999999999999999999999999999\n5\n");

  // ACT
  const auto result = Input::floor_count();

  // ASSERT
  EXPECT_EQ(result, 5);
}