#pragma once

#include <format>
#include <iostream>
#include <limits>

#include "Exceptions.hpp"
#include "constants.hpp"
#include "types.hpp"

class Input {
public:
  static core::FloorNum floor_count() {
    while (true) {
      try {
        int floors{};
        std::cout << "Enter number of floors: ";
        if (!(std::cin >> floors)) {
          throw InputException("Invalid input");
        }
        if (floors < MIN_COUNT_FLOORS || floors > MAX_COUNT_FLOORS) {
          throw InputException(
              std::format("Number of floors must be between {} and {}",
                          MIN_COUNT_FLOORS, MAX_COUNT_FLOORS));
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return static_cast<core::FloorNum>(floors);
      } catch (const InputException &e) {
        std::cout << e.what() << '\n';
        if (std::cin.fail()) {
          std::cin.clear();
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }
  }
};