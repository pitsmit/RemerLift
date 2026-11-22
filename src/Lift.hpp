#pragma once

#include <algorithm>
#include <format>
#include <iostream>
#include <vector>

#include "Command.hpp"
#include "types.hpp"

enum class Direction { NoTasks, TargetIsHigher, TargetIsLower = -1 };

class Lift {
  core::FloorNum current_floor = 1;
  Direction direction = Direction::NoTasks;

  std::vector<core::FloorNum> tasks_higher;
  std::vector<core::FloorNum> tasks_lower;

  void where_is_cabin() const {
    static bool no_tasks_flag = false;
    if (direction == Direction::NoTasks && no_tasks_flag) {
      return;
    }
    std::cout << std::format("Cabin is on floor {}, and going {}. <Press any "
                             "key to enter new command>",
                             current_floor,
                             direction == Direction::NoTasks ? "nowhere"
                             : direction == Direction::TargetIsHigher ? "up"
                                                                      : "down")
              << std::endl;
    if (direction == Direction::NoTasks) {
      no_tasks_flag = true;
    } else {
      no_tasks_flag = false;
    }
  }

  void stopping_message() const {
    std::cout << std::format("Cabin stopped on floor {}.", current_floor)
              << std::endl;
    std::cout << "Doors are closed." << std::endl;
    std::cout << "Doors are opening..." << std::endl;
    std::cout << "Doors are closing..." << std::endl;
    std::cout << "Doors are closed. Cabin is moving again." << std::endl;
  }

  void update_current_floor() noexcept {
    current_floor += static_cast<int>(direction);
  }

  void update_state() noexcept {
    if (!tasks_higher.empty() && !tasks_lower.empty()) {
      return;
    }
    if (tasks_higher.empty() && tasks_lower.empty()) {
      direction = Direction::NoTasks;
    } else if (!tasks_higher.empty()) {
      direction = Direction::TargetIsHigher;
    } else if (!tasks_lower.empty()) {
      direction = Direction::TargetIsLower;
    }
  }

public:
  Lift() = default;
  Direction get_state() const noexcept { return direction; }
  void set_state(Direction new_direction) noexcept {
    direction = new_direction;
  }
  core::FloorNum get_current_floor() const noexcept { return current_floor; }
  void set_current_floor(core::FloorNum floor) noexcept {
    current_floor = floor;
  }

  void push_back_to_tasks_higher(core::FloorNum floor) {
    if (std::ranges::find(tasks_higher, floor) == tasks_higher.end()) {
      tasks_higher.push_back(floor);
    }
  }

  void push_back_to_tasks_lower(core::FloorNum floor) {
    if (std::ranges::find(tasks_lower, floor) == tasks_lower.end()) {
      tasks_lower.push_back(floor);
    }
  }

  void add_to_tasks_higher(core::FloorNum floor) {
    if (std::ranges::find(tasks_higher, floor) != tasks_higher.end()) {
      return;
    }
    auto it = std::lower_bound(tasks_higher.begin(), tasks_higher.end(), floor);
    tasks_higher.insert(it, floor);
  }

  void add_to_tasks_lower(core::FloorNum floor) {
    if (std::ranges::find(tasks_lower, floor) != tasks_lower.end()) {
      return;
    }
    auto it = std::lower_bound(tasks_lower.begin(), tasks_lower.end(), floor,
                               std::greater{});
    tasks_lower.insert(it, floor);
  }

  void next_step() {
    where_is_cabin();
    update_current_floor();
    if (direction == Direction::TargetIsHigher) {
      if (!tasks_higher.empty() && current_floor == tasks_higher.front()) {
        tasks_higher.erase(tasks_higher.begin());
        stopping_message();
      }
    } else if (direction == Direction::TargetIsLower) {
      if (!tasks_lower.empty() && current_floor == tasks_lower.front()) {
        tasks_lower.erase(tasks_lower.begin());
        stopping_message();
      }
    }
    update_state();
  }

  bool has_no_tasks_higher_than(const Command &cmd) const noexcept {
    return cmd.floor > tasks_higher.back();
  }
};