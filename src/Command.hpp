#pragma once

#include <algorithm>
#include <string>

#include "Exceptions.hpp"
#include "constants.hpp"
#include "types.hpp"

enum class CommandType {
  OnFloor = FLOOR_COMMAND_PREFIX,
  InCabin = CABIN_COMMAND_PREFIX
};

class Command {
  CommandType type;
  core::FloorNum floor;

public:
  Command(const std::string &input) { // Команда должна начинаться с 'F' или
                                      // 'C', за которым следует номер этажа
    if (input.size() < MIN_COMMAND_LENGTH ||
        (input[0] != FLOOR_COMMAND_PREFIX &&
         input[0] != CABIN_COMMAND_PREFIX)) {
      throw InputException("Invalid command");
    }
    const auto floor_str = input.substr(1);
    if (floor_str.empty() ||
        !std::all_of(floor_str.begin(), floor_str.end(),
                     [](unsigned char c) { return std::isdigit(c); })) {
      throw InputException("Invalid floor");
    }
    type = static_cast<CommandType>(input[0]);
    floor = static_cast<core::FloorNum>(std::stoi(floor_str));
  }
  CommandType get_type() const { return type; }
  core::FloorNum get_floor() const { return floor; }

  bool has_valid_floor(const core::FloorNum count_floors) const {
    return floor >= MIN_FLOOR_VALUE && floor <= count_floors;
  }
};