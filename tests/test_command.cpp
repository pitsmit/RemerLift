#include <gtest/gtest.h>

#include <string>

#include "Command.hpp"
#include "Exceptions.hpp"

TEST(CommandTest, CreatesFloorCommand) {
  // ARRANGE
  const std::string input = "F5";

  // ACT
  const Command command(input);

  // ASSERT
  EXPECT_EQ(command.get_type(), CommandType::OnFloor);
  EXPECT_EQ(command.get_floor(), 5);
}

TEST(CommandTest, CreatesCabinCommand) {
  // ARRANGE
  const std::string input = "C7";

  // ACT
  const Command command(input);

  // ASSERT
  EXPECT_EQ(command.get_type(), CommandType::InCabin);
  EXPECT_EQ(command.get_floor(), 7);
}

TEST(CommandTest, CreatesCommandForFloorOne) {
  // ARRANGE
  const std::string input = "F1";

  // ACT
  const Command command(input);

  // ASSERT
  EXPECT_EQ(command.get_floor(), 1);
}

TEST(CommandTest, CreatesCommandWithMultipleDigitFloor) {
  // ARRANGE
  const std::string input = "F15";

  // ACT
  const Command command(input);

  // ASSERT
  EXPECT_EQ(command.get_floor(), 15);
}

TEST(CommandTest, ThrowsOnInvalidPrefix) {
  // ARRANGE
  const std::string input = "X5";

  // ACT && ASSERT
  EXPECT_THROW(Command{input}, InputException);
}

TEST(CommandTest, ThrowsOnLowercasePrefix) {
  // ARRANGE
  const std::string input = "f5";

  // ACT && ASSERT
  EXPECT_THROW(Command{input}, InputException);
}

TEST(CommandTest, ThrowsOnEmptyInput) {
  // ARRANGE
  const std::string input = "";

  // ACT && ASSERT
  EXPECT_THROW(Command{input}, InputException);
}

TEST(CommandTest, ThrowsOnSingleCharacterInput) {
  // ARRANGE
  const std::string input = "F";

  // ACT && ASSERT
  EXPECT_THROW(Command{input}, InputException);
}

TEST(CommandTest, ThrowsWhenFloorIsMissing) {
  // ARRANGE
  const std::string input = "F";

  // ACT && ASSERT
  EXPECT_THROW(Command{input}, InputException);
}

TEST(CommandTest, ThrowsOnAlphabeticFloor) {
  // ARRANGE
  const std::string input = "Fabc";

  // ACT && ASSERT
  EXPECT_THROW(Command{input}, InputException);
}

TEST(CommandTest, ThrowsOnMixedAlphabeticAndNumericFloor) {
  // ARRANGE
  const std::string input = "F12a";

  // ACT && ASSERT
  EXPECT_THROW(Command{input}, InputException);
}

TEST(CommandTest, ThrowsOnNegativeFloor) {
  // ARRANGE
  const std::string input = "F-5";

  // ACT && ASSERT
  EXPECT_THROW(Command{input}, InputException);
}

TEST(CommandTest, ThrowsOnPositiveSignInFloor) {
  // ARRANGE
  const std::string input = "F+5";

  // ACT && ASSERT
  EXPECT_THROW(Command{input}, InputException);
}

TEST(CommandTest, ThrowsOnWhitespaceInFloor) {
  // ARRANGE
  const std::string input = "F 5";

  // ACT && ASSERT
  EXPECT_THROW(Command{input}, InputException);
}

TEST(CommandTest, ThrowsOnWhitespaceAfterFloor) {
  // ARRANGE
  const std::string input = "F5 ";

  // ACT && ASSERT
  EXPECT_THROW(Command{input}, InputException);
}

TEST(CommandTest, AcceptsMinimumFloor) {
  // ARRANGE
  const Command command("F1");

  // ACT
  const bool result = command.has_valid_floor(10);

  // ASSERT
  EXPECT_TRUE(result);
}

TEST(CommandTest, AcceptsMaximumFloor) {
  // ARRANGE
  const Command command("F10");

  // ACT
  const bool result = command.has_valid_floor(10);

  // ASSERT
  EXPECT_TRUE(result);
}

TEST(CommandTest, RejectsFloorAboveMaximum) {
  // ARRANGE
  const Command command("F11");

  // ACT
  const bool result = command.has_valid_floor(10);

  // ASSERT
  EXPECT_FALSE(result);
}

TEST(CommandTest, RejectsFloorBelowMinimum) {
  // ARRANGE
  const Command command("F0");

  // ACT
  const bool result = command.has_valid_floor(10);

  // ASSERT
  EXPECT_FALSE(result);
}

TEST(CommandTest, AcceptsCabinCommandWithinRange) {
  // ARRANGE
  const Command command("C5");

  // ACT
  const bool result = command.has_valid_floor(10);

  // ASSERT
  EXPECT_TRUE(result);
}

TEST(CommandTest, RejectsCabinCommandOutsideRange) {
  // ARRANGE
  const Command command("C15");

  // ACT
  const bool result = command.has_valid_floor(10);

  // ASSERT
  EXPECT_FALSE(result);
}

TEST(CommandTest, AcceptsFloorWithLeadingZeros) {
  // ARRANGE
  const Command command("F005");

  // ACT
  const auto floor = command.get_floor();

  // ASSERT
  EXPECT_EQ(floor, 5);
}

TEST(CommandTest, ThrowsOnIntegerOverflow) {
  // ARRANGE
  const std::string input = "F999999999999999999999999999";

  // ACT && ASSERT
  EXPECT_THROW(Command{input}, std::out_of_range);
}
