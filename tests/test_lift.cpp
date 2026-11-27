#include <gtest/gtest.h>

#include "Lift.hpp"

TEST(LiftTest, AddsHigherTask) {
  // ARRANGE
  Lift lift;

  // ACT
  lift.add_to_tasks_higher(5);

  // ASSERT
  EXPECT_EQ(lift.count_tasks_higher(), 1);
}

TEST(LiftTest, DoesNotAddDuplicateHigherTask) {
  // ARRANGE
  Lift lift;
  lift.add_to_tasks_higher(5);

  // ACT
  lift.add_to_tasks_higher(5);

  // ASSERT
  EXPECT_EQ(lift.count_tasks_higher(), 1);
}

TEST(LiftTest, AddsMultipleHigherTasks) {
  // ARRANGE
  Lift lift;

  // ACT
  lift.add_to_tasks_higher(3);
  lift.add_to_tasks_higher(5);
  lift.add_to_tasks_higher(7);

  // ASSERT
  EXPECT_EQ(lift.count_tasks_higher(), 3);
}

TEST(LiftTest, AddsLowerTask) {
  // ARRANGE
  Lift lift;

  // ACT
  lift.add_to_tasks_lower(3);

  // ASSERT
  EXPECT_EQ(lift.count_tasks_lower(), 1);
}

TEST(LiftTest, DoesNotAddDuplicateLowerTask) {
  // ARRANGE
  Lift lift;
  lift.add_to_tasks_lower(3);

  // ACT
  lift.add_to_tasks_lower(3);

  // ASSERT
  EXPECT_EQ(lift.count_tasks_lower(), 1);
}

TEST(LiftTest, AddsMultipleLowerTasks) {
  // ARRANGE
  Lift lift;

  // ACT
  lift.add_to_tasks_lower(7);
  lift.add_to_tasks_lower(5);
  lift.add_to_tasks_lower(3);

  // ASSERT
  EXPECT_EQ(lift.count_tasks_lower(), 3);
}

TEST(LiftTest, PushBackHigherTaskDoesNotAddDuplicate) {
  // ARRANGE
  Lift lift;

  // ACT
  lift.push_back_to_tasks_higher(5);
  lift.push_back_to_tasks_higher(5);

  // ASSERT
  EXPECT_EQ(lift.count_tasks_higher(), 1);
}

TEST(LiftTest, PushBackLowerTaskDoesNotAddDuplicate) {
  // ARRANGE
  Lift lift;

  // ACT
  lift.push_back_to_tasks_lower(5);
  lift.push_back_to_tasks_lower(5);

  // ASSERT
  EXPECT_EQ(lift.count_tasks_lower(), 1);
}

TEST(LiftTest, HigherAndLowerTasksAreStoredSeparately) {
  // ARRANGE
  Lift lift;

  // ACT
  lift.add_to_tasks_higher(7);
  lift.add_to_tasks_lower(3);

  // ASSERT
  EXPECT_EQ(lift.count_tasks_higher(), 1);
  EXPECT_EQ(lift.count_tasks_lower(), 1);
}

TEST(LiftTest, AddHigherTaskDoesNotAffectLowerTasks) {
  // ARRANGE
  Lift lift;
  lift.add_to_tasks_lower(3);

  // ACT
  lift.add_to_tasks_higher(7);

  // ASSERT
  EXPECT_EQ(lift.count_tasks_higher(), 1);
  EXPECT_EQ(lift.count_tasks_lower(), 1);
}

TEST(LiftTest, AddLowerTaskDoesNotAffectHigherTasks) {
  // ARRANGE
  Lift lift;
  lift.add_to_tasks_higher(7);

  // ACT
  lift.add_to_tasks_lower(3);

  // ASSERT
  EXPECT_EQ(lift.count_tasks_higher(), 1);
  EXPECT_EQ(lift.count_tasks_lower(), 1);
}

TEST(LiftTest, MovesUpOneFloor) {
  // ARRANGE
  Lift lift;
  lift.set_delay_duration(std::chrono::milliseconds{0});
  lift.add_to_tasks_higher(3);
  lift.send_up();

  // ACT
  lift.next_step();

  // ASSERT
  EXPECT_EQ(lift.get_current_floor(), 2);
  EXPECT_EQ(lift.count_tasks_higher(), 1);
  EXPECT_TRUE(lift.goes_up());
}

TEST(LiftTest, ReachesHigherTaskAndStops) {
  // ARRANGE
  Lift lift;
  lift.set_delay_duration(std::chrono::milliseconds{0});
  lift.add_to_tasks_higher(3);
  lift.send_up();

  // ACT
  lift.next_step();
  lift.next_step();

  // ASSERT
  EXPECT_EQ(lift.get_current_floor(), 3);
  EXPECT_EQ(lift.count_tasks_higher(), 0);
  EXPECT_TRUE(lift.stay());
}

TEST(LiftTest, MovesToFirstHigherTaskAndKeepsMovingToNext) {
  // ARRANGE
  Lift lift;
  lift.set_delay_duration(std::chrono::milliseconds{0});
  lift.add_to_tasks_higher(3);
  lift.add_to_tasks_higher(5);
  lift.send_up();

  // ACT
  lift.next_step();
  lift.next_step();

  // ASSERT
  EXPECT_EQ(lift.get_current_floor(), 3);
  EXPECT_EQ(lift.count_tasks_higher(), 1);
  EXPECT_TRUE(lift.goes_up());

  // ACT
  lift.next_step();
  lift.next_step();

  // ASSERT
  EXPECT_EQ(lift.get_current_floor(), 5);
  EXPECT_EQ(lift.count_tasks_higher(), 0);
  EXPECT_TRUE(lift.stay());
}

TEST(LiftTest, ChangesDirectionAfterReachingHighestTask) {
  // ARRANGE
  Lift lift;
  lift.set_delay_duration(std::chrono::milliseconds{0});

  lift.add_to_tasks_higher(3);
  lift.send_up();

  // ACT
  lift.next_step();
  lift.next_step();

  // ASSERT
  EXPECT_EQ(lift.get_current_floor(), 3);
  EXPECT_TRUE(lift.stay());

  // ARRANGE
  lift.add_to_tasks_lower(1);
  lift.send_down();

  // ACT
  lift.next_step();
  lift.next_step();

  // ASSERT
  EXPECT_EQ(lift.get_current_floor(), 1);
  EXPECT_EQ(lift.count_tasks_lower(), 0);
  EXPECT_TRUE(lift.stay());
}

TEST(LiftTest, MovesDownAfterGoingUp) {
  // ARRANGE
  Lift lift;
  lift.set_delay_duration(std::chrono::milliseconds{0});

  lift.add_to_tasks_higher(5);
  lift.send_up();

  // ACT
  lift.next_step();
  lift.next_step();
  lift.next_step();
  lift.next_step();

  // ASSERT
  EXPECT_EQ(lift.get_current_floor(), 5);
  EXPECT_TRUE(lift.stay());

  // ARRANGE
  lift.add_to_tasks_lower(3);
  lift.send_down();

  // ACT
  lift.next_step();
  lift.next_step();

  // ASSERT
  EXPECT_EQ(lift.get_current_floor(), 3);
  EXPECT_EQ(lift.count_tasks_lower(), 0);
  EXPECT_TRUE(lift.stay());
}