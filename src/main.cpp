#include <iostream>
#include <thread>

#include "AddTaskStrategy.hpp"
#include "Command.hpp"
#include "Exceptions.hpp"
#include "Lift.hpp"
#include "constants.hpp"
#include "types.hpp"

class App {
  core::FloorNum count_floors = 0;
  Lift lift;

  core::FloorNum input_floor_count() {
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
        return static_cast<core::FloorNum>(floors);
      } catch (const InputException &e) {
        std::cout << e.what() << '\n';
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      }
    }
  }

  Command parse_command(const std::string &input) {
    if (input.size() < 2 || (input[0] != 'F' && input[0] != 'C')) {
      throw InputException("Invalid command");
    }
    const auto floor_str = input.substr(1);
    if (floor_str.empty() ||
        !std::all_of(floor_str.begin(), floor_str.end(),
                     [](unsigned char c) { return std::isdigit(c); })) {
      throw InputException("Invalid floor");
    }
    const int floor = std::stoi(floor_str);
    if (floor < 1 || floor > count_floors) {
      throw InputException("Floor is out of range");
    }
    return {.type = static_cast<CommandType>(input[0]),
            .floor = static_cast<core::FloorNum>(floor)};
  }

  void process_command(Lift &lift, Command &command) {
    std::vector<std::unique_ptr<AddTaskStrategy>> strategies;

    strategies.reserve(15);

    strategies.push_back(
        std::make_unique<LiftHasNoTasks_Higher_Strategy>(lift, command));
    strategies.push_back(
        std::make_unique<LiftHasNoTasks_Lower_Strategy>(lift, command));
    strategies.push_back(
        std::make_unique<LiftHasNoTasks_Current_Strategy>(lift, command));

    strategies.push_back(
        std::make_unique<LiftGoesUp_Current_Strategy>(lift, command));
    strategies.push_back(
        std::make_unique<LiftGoesUp_Higher_Cabin_Strategy>(lift, command));
    strategies.push_back(
        std::make_unique<LiftGoesUp_Lower_Cabin_Strategy>(lift, command));
    strategies.push_back(
        std::make_unique<LiftGoesUp_HigherTopTask_OnFloor_Strategy>(lift,
                                                                    command));
    strategies.push_back(
        std::make_unique<LiftGoesUp_LowerTopTask_OnFloor_Strategy>(lift,
                                                                   command));
    strategies.push_back(
        std::make_unique<LiftGoesUp_Lower_OnFloor_Strategy>(lift, command));

    strategies.push_back(
        std::make_unique<LiftGoesDown_Current_Cabin_Strategy>(lift, command));
    strategies.push_back(
        std::make_unique<LiftGoesDown_Higher_Cabin_Strategy>(lift, command));
    strategies.push_back(
        std::make_unique<LiftGoesDown_Lower_Cabin_Strategy>(lift, command));
    strategies.push_back(
        std::make_unique<LiftGoesDown_Current_OnFloor_Strategy>(lift, command));
    strategies.push_back(
        std::make_unique<LiftGoesDown_Higher_OnFloor_Strategy>(lift, command));
    strategies.push_back(
        std::make_unique<LiftGoesDown_Lower_OnFloor_Strategy>(lift, command));

    for (const auto &strategy : strategies) {
      if (strategy->can_add_task()) {
        strategy->add_task();
        return;
      }
    }

    throw NoStrategyException("No suitable strategy found for this command.");
  }

  void wait_for_enter() {
    std::string input;
    while (std::getline(std::cin, input)) {
      std::cout << "Enter command: ";
      if (!std::getline(std::cin, input)) {
        break;
      }
      try {
        auto command = parse_command(input);
        process_command(lift, command);
      } catch (const InputException &e) {
        std::cout << e.what() << '\n';
      }
    }
  }

public:
  App() : count_floors(input_floor_count()) {}
  void run() {
    std::jthread input_thread([this] { wait_for_enter(); });

    while (true) {
      lift.next_step();
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  }
};

int main() {
  App().run();
  return 0;
}