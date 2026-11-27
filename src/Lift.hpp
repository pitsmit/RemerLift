#pragma once

#include <algorithm>
#include <format>
#include <iostream>
#include <thread>
#include <vector>

#include "Exceptions.hpp"
#include "constants.hpp"
#include "types.hpp"

enum class Direction { NoTasks, TargetIsHigher, TargetIsLower };

class Lift {
  core::FloorNum current_floor = START_FLOOR;
  Direction direction = Direction::NoTasks;

  std::vector<core::FloorNum> tasks_higher; // таски на этажи выше текущего
  std::vector<core::FloorNum> tasks_lower;  // таски на этажи ниже текущего

  bool is_muted = false; // если установлен, то лифт не будет выводить
                         // информацию о текущем состоянии

  std::chrono::milliseconds delay_duration{1000};

  // вывод информации о текущем состоянии лифта
  void where_is_cabin() const {
    if (is_muted) {
      return;
    }
    static bool no_tasks_flag =
        false; // флаг, чтобы не выводить сообщение о том, что лифт никуда не
               // едет, если он уже стоял на месте
    if (stay() && no_tasks_flag) {
      return;
    }
    std::cout
        << std::format(
               "Cabin is on floor {}, and going {}. <Press "
               "ctrl+C to mute lift, then write new commmand and press enter>",
               current_floor,
               stay()      ? "nowhere"
               : goes_up() ? "up"
                           : "down")
        << std::endl;
    no_tasks_flag = stay();
  }

  void update_current_floor() {
    if (goes_up()) {
      ++current_floor;
    } else if (goes_down()) {
      --current_floor;
    }
    if (current_floor < MIN_FLOOR_VALUE || current_floor > MAX_COUNT_FLOORS) {
      throw IrreversibleException( // необратимая ошибка, так как лифт не может
                                   // выйти за пределы допустимого диапазона
                                   // этажей
          "Lift cannot go outside the valid floor range.");
    }
  }

  void update_direction() noexcept {
    if (!tasks_higher.empty() && !tasks_lower.empty()) {
      return;
    }
    if (tasks_higher.empty() && tasks_lower.empty()) {
      stop();
    } else if (!tasks_higher.empty()) {
      send_up();
    } else if (!tasks_lower.empty()) {
      send_down();
    }
  }

  // проверяем что лифт достиг цели и удаляем этаж из списка задач, если достиг
  void reach_goal(std::vector<core::FloorNum> &floors) {
    if (!floors.empty() && current_floor == floors.front()) {
      floors.erase(floors.begin());
      stopping_message();
    }
  }

  // хелпер для проверки наличия задачи
  bool has_task(const std::vector<core::FloorNum> &tasks,
                core::FloorNum floor) const {
    return std::ranges::find(tasks, floor) != tasks.end();
  }

public:
  Lift() = default;
  void send_up() noexcept { direction = Direction::TargetIsHigher; }
  void send_down() noexcept { direction = Direction::TargetIsLower; }
  void stop() noexcept { direction = Direction::NoTasks; }
  core::FloorNum get_current_floor() const noexcept { return current_floor; }
  bool goes_down() const noexcept {
    return direction == Direction::TargetIsLower;
  }
  bool goes_up() const noexcept {
    return direction == Direction::TargetIsHigher;
  }
  bool stay() const noexcept { return direction == Direction::NoTasks; }
  size_t count_tasks_higher() const noexcept { return tasks_higher.size(); }
  size_t count_tasks_lower() const noexcept { return tasks_lower.size(); }

  void mute() noexcept { is_muted = true; }
  void unmute() noexcept { is_muted = false; }

  void push_back_to_tasks_higher(core::FloorNum floor) {
    if (has_task(tasks_higher, floor))
      return;
    tasks_higher.push_back(floor);
  }

  void push_back_to_tasks_lower(core::FloorNum floor) {
    if (has_task(tasks_lower, floor))
      return;
    tasks_lower.push_back(floor);
  }

  void add_to_tasks_higher(core::FloorNum floor) {
    if (has_task(tasks_higher, floor))
      return;
    auto it = std::lower_bound(tasks_higher.begin(), tasks_higher.end(), floor);
    tasks_higher.insert(it, floor);
  }

  void add_to_tasks_lower(core::FloorNum floor) {
    if (has_task(tasks_lower, floor))
      return;
    auto it = std::lower_bound(tasks_lower.begin(), tasks_lower.end(), floor,
                               std::greater{});
    tasks_lower.insert(it, floor);
  }

  void stopping_message() const {
    std::cout << std::format("Cabin stopped on floor {}.", current_floor)
              << std::endl;
    delay_to_truth();
    std::cout << "Doors are closed." << std::endl;
    delay_to_truth();
    std::cout << "Doors are opening..." << std::endl;
    delay_to_truth();
    std::cout << "Doors are opened." << std::endl;
    delay_to_truth();
    std::cout << "Doors are closing..." << std::endl;
    delay_to_truth();
    std::cout << "Doors are closed." << std::endl;
    delay_to_truth();
  }

  void next_step() {
    where_is_cabin();
    update_current_floor();
    if (goes_up()) {
      reach_goal(tasks_higher);
    } else if (goes_down()) {
      reach_goal(tasks_lower);
    }
    update_direction();
  }

  bool has_no_tasks_higher_than(const core::FloorNum floor) const noexcept {
    return tasks_higher.empty() || floor >= tasks_higher.back();
  }

  // обертка для задержки
  void delay_to_truth() const { std::this_thread::sleep_for(delay_duration); }

  void set_delay_duration(std::chrono::milliseconds duration) noexcept {
    delay_duration = duration;
  }
};