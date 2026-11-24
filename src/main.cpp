#include <atomic>
#include <csignal>
#include <iostream>

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
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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

  // выбор стратегии добавления задачи в список задач в зависимости от состояния
  // лифта и команды
  void process_command(Lift &lift, Command &command) {
    std::vector<std::unique_ptr<AddTaskStrategy>> strategies;
    strategies.reserve(14);

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
        std::make_unique<LiftGoesDown_Current_Strategy>(lift, command));
    strategies.push_back(
        std::make_unique<LiftGoesDown_Higher_Cabin_Strategy>(lift, command));
    strategies.push_back(
        std::make_unique<LiftGoesDown_Lower_Cabin_Strategy>(lift, command));
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

  // получение команды и дальнейшая обработка
  void read_command() {
    std::string input;
    std::cout << "\nEnter command: ";
    if (!std::getline(std::cin, input)) {
      return;
    }
    try {
      auto command = Command(input);
      if (command.has_valid_floor(count_floors)) {
        process_command(lift, command);
      } else {
        throw InputException("Floor is out of range");
      }
    } catch (const InputException &e) {
      std::cout << e.what() << '\n';
    }
  }

  static void handle_sigint(int) { command_requested = true; } // обработчик

public:
  App() : count_floors(input_floor_count()) {}

  static std::atomic_bool command_requested;

  // основной цикл работы приложения
  void run() {
    std::signal(SIGINT, handle_sigint);
    while (true) {
      lift.next_step();
      if (command_requested.exchange(
              false)) { // возвращает текущее значение и сбрасывает в false
        read_command();
      }
      lift.delay_to_truth();
    }
  }
};

std::atomic_bool App::command_requested = false;

int main() {
  App().run();
  return 0;
}